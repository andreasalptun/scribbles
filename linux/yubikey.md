# Yubikey

I'm running Linux Mint, Cinnamon edition. This should work on any Debian-ish distro.

## Using GPG key as SSH key

Make sure you are running gpg2.

Add to `~/.gnupg/gpg.conf` (Create if non-existing)
```
personal-digest-preferences SHA256
cert-digest-algo SHA256
default-preference-list SHA512 SHA384 SHA256 SHA224 AES256 AES192 AES CAST5 ZLIB BZIP2 ZIP Uncompressed
keyserver hkp://keys.gnupg.net
```

Add to `~/.gnupg/gpg-agent.conf` (Create if non-existing)
```
enable-ssh-support
```

Specify which gpg key to use. Find keygrip for your private auth key, [A], using `gpg -K --with-keygrip`. Add the keygrip hex-value to `~/.gnupg/sshcontrol`

Add to `~/.zshrc` (or `.bashrc`)
```
export SSH_AUTH_SOCK=$(gpgconf --list-dirs agent-ssh-socket)
gpgconf --launch gpg-agent
```

Restart agent
```
gpg-connect-agent killagent /bye
gpg-connect-agent /bye
```

Get your new public ssh key using `ssh-add -L`, use the one that ends with cardno:XXX. Or by using `ssh-copy-id`.

## PAM OTP auth for sudo

Enables sudo auth by just pressing the yubikey button. Requires internet connection for OTP validation. Falls back to user password.

### Install
```
sudo add-apt-repository ppa:yubico/stable
sudo apt-get update
sudo apt-get install libpam-yubico
```

### Create Authorization Mapping

Create a file `~/.yubico/authorized_yubikeys` with contents `username:yubikey-token-id`

The token id is the first 12 chars of any OTP from your yubikey.

### Activate pam module for sudo

In /etc/pam.d/sudo:

`auth       sufficient pam_yubico.so id=<client-id> key=<client-secret>`

Add `debug` to module parameters to get verbose.

Get id and secret from [YubiCloud](https://upgrade.yubico.com/getapikey/).
