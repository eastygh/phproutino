# phproutino
php extension for routino project

PHP ROUTINO extension install
------------------------------

```bash 
phpize
./configure
make
make install
```

```php 
$r=new Routino("/var/data/osm/","khm"); // open databse in /var/data/osm/ using prefix khm - for example Khanty-Mansiysk
$route=$r->calculate([['lat'=>60.9761133,'lon'=>69.0239906],['lat'=>61.0024578,'lon'=>69.0178842]]));
var_dump($route)
$r->close();
```
