# phproutino
php7 extension for routino project.

Routino project - http://routino.org/

PHP ROUTINO extension install
------------------------------

1) Install routino project

2) Install phproutino
```bash 
sudo apt-get install php-dev 
phpize
./configure
make
sudo make install
```
3) Make local routino database using osm data. See http://routino.org/

4) Enable phproutino extension.

For debian example:
```bash 
phpenmod routino
```
5) Using extension in php

Example test.php
```php 
$r=new Routino("/var/data/osm/","khm"); // open database in /var/data/osm/ and khm prefix
$route=$r->calculate([['lat'=>60.9761133,'lon'=>69.0239906],['lat'=>61.0024578,'lon'=>69.0178842]]));
var_dump($route)
$r->close();
```

Attention:
-----------------------
Project is in early alpha stage and support only php7
