
# Setting up a git repo

### Prerequisites

```bash
addgroup git
adduser --ingroup git --shell bash git-username
```

### Create repo
```bash
mkdir -p /var/git/<your-app>.git
cd /var/git/<your-app>.git
git init --bare
chgrp -R git .
chmod -R g+rwX .  # Capital X = directories only
find . -type d -exec chmod g+s {} \;
git config core.sharedRepository group
```
