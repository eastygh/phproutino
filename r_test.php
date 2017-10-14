<?php

function Test(){
    $r=new Routino("/home/easty/data/","khmao");  
    var_dump($r->getLastError());
    //$r->Calculate();
    var_dump($r->getLanguagesEx());
    var_dump($r->getDir());
    var_dump($r->getPrefix());
    var_dump($r->getLng());
    var_dump($r->getProfiles());
    var_dump($r->getProfile());
    $cc=[];
    $cc[1]=['lat'=>60.9761133,'lon'=>69.0239906];
    $cc[2]=['lat'=>61.0024578,'lon'=>69.0178842];
    $cc[3]=['lat'=>61.0100402,'lon'=>69.0568554];
    var_dump($r->Calculate($cc));
    var_dump($r->getLastError());
//    var_dump($r->Calculate([['lat'=>60.9761133,'lon'=>69.0239906],['lat'=>61.0024578,'lon'=>69.0178842]]));
//    var_dump($r->Calculate([]));
    $r->Close();
}

//Routino::setLngFile("QQQ");
//echo routino_opendatabase("/home/easty/data/","khmao");
echo "\n";
//Routino::setProfilesFName("/home/easty/Загрузки/profiles.xml");
Test();

Routino::setProfilesFName("/home/easty/Загрузки/profiles.xml");

//echo ROUTINO_ROUTE_LIST_TEXT_ALL."\n";
//$r=new Routino("/home/easty/data/","khmao");
//$r->setLngFile("QQQ");

class NR extends Routino{

    function __construct(){
        parent::__construct();
    }

}

?>