# Yubikey

I'm running Linux Mint, Cinnamon edition. This should work on any Debian-ish distro.

## Using GPG key as SSH key

Make sure you are running gpg2.

Add to `~/.gnupg/gpg.conf` (Create if non-existing)

    personal-digest-preferences SHA256
    cert-digest-algo SHA256
    default-preference-list SHA512 SHA384 SHA256 SHA224 AES256 AES192 AES CAST5 ZLIB BZIP2 ZIP Uncompressed
    keyserver hkp://keys.gnupg.net

Add to `~/.gnupg/gpg-agent.conf` (Create if non-existing)

    enable-ssh-support

Specify which gpg key to use. Find keygrip for your private auth key, [A], using `gpg -K --with-keygrip`. Add the keygrip hex-value to `~/.gnupg/sshcontrol`

Add to `~/.zshrc` (or `.bashrc`)

    export SSH_AUTH_SOCK=$(gpgconf --list-dirs agent-ssh-socket)
    gpgconf --launch gpg-agent

Restart agent

    gpg-connect-agent killagent /bye
    gpg-connect-agent /bye

Get your new public ssh key using `ssh-add -L`, use the one that ends with cardno:XXX. Or by using `ssh-copy-id`.

## PAM OTP auth for sudo

Enables sudo auth by just pressing the yubikey button. Requires internet connection for OTP validation. Falls back to user password.

### Install

    sudo add-apt-repository ppa:yubico/stable
    sudo apt-get update
    sudo apt-get install yubikey-manager libpam-yubico

Make sure OTP is enabled on your yubikey using [ykman](https://support.yubico.com/support/solutions/articles/15000012643-yubikey-manager-cli-ykman-user-manual).

### Create Authorization Mapping

Create a file `~/.yubico/authorized_yubikeys` with contents `username:yubikey-token-id`

The token id is the first 12 chars of any OTP from your yubikey.

### Activate pam module for sudo

In `/etc/pam.d/sudo`, add just before `@include common-auth`:

`auth       sufficient pam_yubico.so id=<client-id> key=<client-secret>`

Add `debug` to module parameters to get verbose.

Get id and secret from [YubiCloud](https://upgrade.yubico.com/getapikey/).

## Lock screen when yubikey is removed

Use [udev](https://opensource.com/article/18/11/udev) to run a script when a yubikey is removed.

### Create the lock screen script

Scripts executed by udev are run as root but the screen must be locked by current user. Other scripts that I have found didnt't work properly if more than one user is logged into the same computer. This is my attempt at it, much more compact and understandable IMO. There might be edge cases that doesn't work though.

Copy the following script to `/usr/local/bin/yubikey-lock-screen.sh`.

```bash
#!/bin/bash

# Get current tty
TTY=`cat /sys/class/tty/tty0/active`

# Use pinky(finger) to find which user is logged in to current tty
USER=`pinky -fq | grep -m1 $TTY | awk '{print $1}'`

# Logging event
echo "Locking screen for $USER @ $TTY" >> /var/log/udev-yubikey.log

# Lock screen
su "$USER" -c "/usr/bin/cinnamon-screensaver-command -l"
```

If you are using another desktop environment, the last command must be modified accordingly, i.e. `gnome-screensaver-command -l` for Ubuntu.

### Hook up the script to a udev event

Copy the yubikey udev template to enable it

    sudo cp /lib/udev/rules.d/69-yubikey.rules /etc/udev/rules.d/

Append the following line to the template. If this rule doesn't work on your config then use `sudo udevadm monitor -u -p` to monitor udev events.

    ACTION=="remove", SUBSYSTEM=="hid", ENV{HID_NAME}=="Yubico YubiKey*", RUN+="/usr/local/bin/yubikey-lock-screen.sh"

Finally, reload the rules with `sudo udevadm control --reload-rules`
