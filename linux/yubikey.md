# Yubikey


## Using GPG key as SSH key

TODO


## PAM OTP auth for sudo

Enables sudo auth by just pressing the yubikey button. Requires internet connection for OTP validation. Falls back to user password.

### 1. Install
```
sudo add-apt-repository ppa:yubico/stable
sudo apt-get update
sudo apt-get install libpam-yubico
```

### 2. Authorization Mapping

Create a file `~/.yubico/authorized_yubikeys` with contents `username:yubikey-token-id`

The token id is the first 12 chars of any OTP

### 3. Activate pam module for sudo

In /etc/pam.d/sudo:

`auth       sufficient pam_yubico.so id=<client-id> key=<client-secret>`

Add `debug` to module parameters to get verbose.

Get id and secret from [YubiCloud](https://upgrade.yubico.com/getapikey/).
