# phproutino
php7 extension for routino project
Routino project - http://routino.org/

PHP ROUTINO extension install
------------------------------

1) Install routino project

2) Install phproutino
```bash 
phpize
./configure
make
sudo make install
```
3) Make local routino database using osm data. See http://routino.org/

4) Using extension in php

Example test.php
```php 
$r=new Routino("/var/data/osm/","khm"); // open databse in /var/data/osm/ and khm prefix
$route=$r->calculate([['lat'=>60.9761133,'lon'=>69.0239906],['lat'=>61.0024578,'lon'=>69.0178842]]));
var_dump($route)
$r->close();
```
