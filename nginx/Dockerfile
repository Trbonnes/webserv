FROM	alpine

RUN		apk update \
		&& apk add nginx openssh \
		&& rm  -rf /tmp/* /var/cache/apk/*

RUN		apk add netcat-openbsd		

RUN		adduser -D -g 'www' www \
		&& mkdir /www \
		&& chown -R www:www /var/lib/nginx \
		&& chown -R www:www /www \
		&& mv /etc/nginx/nginx.conf /etc/nginx/nginx.conf.orig \ 
		&& mkdir -p /run/nginx


RUN		adduser -D user
RUN		echo "user:password"|chpasswd
RUN 	ssh-keygen -A

RUN		apk add openssl \
		&& openssl req -x509 -nodes -days 365 -subj \
		"/C=CA/ST=QC/O=Company, Inc./CN=localhost" \
		-addext "subjectAltName=DNS:localhost" -newkey rsa:2048 \
		-keyout /etc/ssl/private/nginx-selfsigned.key \
		-out /etc/ssl/certs/nginx-selfsigned.crt;

COPY 	srcs/sshd_config /etc/ssh/sshd_config

RUN		apk add supervisor
COPY 	srcs/supervisord.conf /etc/supervisord.conf


COPY	srcs/nginx.conf /etc/nginx

COPY	srcs/nginx.conf /etc/nginx/conf.d

COPY	srcs/index.html /www

COPY 	srcs/code.css /www

COPY	srcs/start.sh .

RUN 	chmod +x start.sh

VOLUME 	/www

EXPOSE	80 443 22

CMD		./start.sh ; /usr/bin/supervisord -c /etc/supervisord.conf
