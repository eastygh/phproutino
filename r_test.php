<?php

function Test(){
    $r=new Routino("/home/easty/data/","khmao");  
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
    var_dump($r->Calculate([['lat'=>60.9761133,'lon'=>69.0239906],['lat'=>61.0024578,'lon'=>69.0178842]]));
    var_dump($r->Calculate([]));
    $r->Close();
}

//echo routino_opendatabase("/home/easty/data/","khmao");
echo "\n";
Test();
//$r=new Routino("/home/easty/data/","khmao");

class NR extends Routino{

    function __construct(){
        parent::__construct();
    }

}

?>