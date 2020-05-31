
# Git Deploy

## Prerequisites

Create a git user in a git group if not exists.

Setup folders:

```bash
# as root
mkdir /var/git
chgrp git /var/git
chmod g+w /var/git

mkdir -p /var/tmp/git-deploy/
chgrp -R git /var/tmp/git-deploy/
chmod g+w /var/tmp/git-deploy/

mkdir -p /var/node/git-deploy/<your-project>/
chgrp -R git /var/node/git-deploy/<your-project>/
chmod g+w /var/node/git-deploy/<your-project>/
```

## Create a repo for project

```bash
# as git user
cd /var/git
git init --bare <your-project>.git
cd <your-project>.git/hooks
touch post-receive
chmod +x post-receive
```

## Create a hook script

This goes into post-receive, modify it as needed:

```bash
#!/usr/bin/env bash

# project/
#   webapp
#   backend
#   nginx

# [ DEPLOY SETTINGS ]
SERVER=my-server-addr
PROJECT=my-project
BACKEND_ARGS="-a localhost"
# [ / ]

# Directories
REPO="/var/git/$PROJECT.git"
TEMP="/var/tmp/git-deploy/$PROJECT.$RANDOM"
TARGET_WEB="/var/www/$PROJECT"
TARGET_BACKEND="/var/node/git-deploy/$PROJECT"
TARGET_NGINX="/etc/nginx/sites-available"

# Checkout release branch
rm -rf $TEMP
mkdir -p $TEMP
git --work-tree=$TEMP --git-dir=$REPO checkout -q -f release

# Build webapp
cd $TEMP/webapp

# [ WEBAPP INSTALL COMMANDS ]
npm install
npm run css-build
npm run build
# [ / ]

# Build backend
cd $TEMP/backend

# [ BACKEND INSTALL COMMANDS ]
npm install
BACKEND_DEPS=common
# [ / ]

# Copy nginx config
cp $TEMP/nginx/$PROJECT.conf $TARGET_NGINX/

# Stop backend
pm2 stop $PROJECT -s

# Install web
mkdir -p -m=750 $TARGET_WEB
rm -rf $TARGET_WEB/*
cp -r $TEMP/$WEB/dist/* $TARGET_WEB/
chgrp -R www-data $TARGET_WEB
chmod -R o-rwx $TARGET_WEB/*

# Install backend
mkdir -p -m=750 $TARGET_BACKEND
cd $TARGET_BACKEND
rm -rf $TARGET_BACKEND/*

cp -r $TEMP/backend .
BACKEND_DEPS=$(echo $BACKEND_DEPS | tr "," "\n")
for BACKEND_DEP in $BACKEND_DEPS
do
  cp -r $TEMP/$BACKEND_DEP .
done
chmod -R o-rwx *

# Start backend
cd backend
pm2 start index.js --name $PROJECT -- $BACKEND_ARGS

# Clean up
rm -rf $TEMP

# Reload nginx notice
echo "RELOAD NGINX: ssh root@$SERVER \"systemctl reload nginx.service\""
```

## Local setup

`git remote add deploy ssh://<user>@<host>/var/git/<your-project>.git`

### Push from local

Deploy local master to deploy/master
`git push deploy master`

Deploy a specific commit to a branch called release
`git push deploy tag-v0.5.2:release`

Deploy current branch to a branch called release
`git push deploy HEAD:release`


## Get pushed branch/ref name

```bash
#!/bin/bash
while read oldrev newrev refname
do
  branch=$(git rev-parse --symbolic --abbrev-ref $refname)
  echo $branch
    if [ "deploy" = "$branch" ]; then
	     # do stuff
    fi
done
```

https://medium.com/@francoisromain/vps-deploy-with-git-fea605f1303b
