<?php

$cfgFile = fopen("configs/cConfig.txt", "w") or die("Can't open file");
fwrite($cfgFile, "Input 1: ".$_POST['test1']."\n");
fwrite($cfgFile, "Input 2: ".$_POST['test2']."\n");
fclose($cfgFile);


?>
