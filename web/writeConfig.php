<?php

$cfgFile = fopen("configs/cConfig.txt", "w") or die("Can't open file");
fwrite($cfgFile, "Hue Min: 0\n");
fwrite($cfgFile, "Hue Max: 0\n");
fwrite($cfgFile, "Saturation Min: 0\n");
fwrite($cfgFile, "Saturation Max: 0\n");
fwrite($cfgFile, "Value Min: 0\n");
fwrite($cfgFile, "Value Max: 0\n");
fwrite($cfgFile, "Filter Background: ".$_POST['fBack']."\n");
fclose($cfgFile);


?>
