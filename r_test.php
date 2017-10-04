<?php

function Test(){
    $r=new Routino("/home/easty/data/","khmao");  
    $m=new Routino("/home/easty/data/","khmao");  
    $r->Close();  
    $r->Open("/home/easty/data/","khmao");
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