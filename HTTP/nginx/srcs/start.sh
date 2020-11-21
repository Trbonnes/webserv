# adduser -D "$SSH_USER"
#!/bin/sh

# echo "$SSH_USER:$SSH_PASSWORD" | chpasswd

# /usr/sbin/sshd -D -e
# nginx -g "daemon off;"

mkdir -p /var/run/nginx

adduser --disabled-password ${SSH_USER}
echo "${SSH_USER}:${SSH_PASSWORD}" | chpasswd
echo "user = ${SSH_USER} pass = ${SSH_PASSWORD}"