# Linux commands

## Add users
`useradd username`
User group: `--ingroup git`, Shell: `--shell bash`

#### Give sudo privileges by adding to sudoers
`usermod -aG sudo username`

## Writing an .iso to a usb thumb drive
`sudo umount /dev/sdb`
`sudo dd bs=4M if=debian-live-9.9.0-amd64-cinnamon.iso of=/dev/sdb conv=fdatasync`

## Replace text in large files
`sed --in-place 's/old text/new text/g' file`

## Curl
`curl -v -b my-jar -L -d "username=andreas&password=mysecret" hostname:8784/login`

-L follow redirects
-b use cookie jar
-c create cookie jar
-d post data

JSON:

curl -v -H "Content-Type: application/json" -X PUT -d '{"serialNumber":"qwerty5"}' URL | jq

## Flush DNS cache
`systemd-resolve --flush-caches`

## Restart trackpad
sudo modprobe -r psmouse && sudo modprobe psmouse
