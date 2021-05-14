<?php
$file_url = "images/capture.jpg";
// Delete previous capture
unlink($file_url);
// Prompt application to capture an image
$comsFile = fopen("configs/startCap.txt", "w") or die("Can't open file");
fwrite($comsFile, "1");
fclose($comsFile);
// Wait for new image
while(!file_exists($file_url)){
    sleep(1);
}
// Send image
header("Content-Type: application/jpg");
header("Content-Transfer-Encoding: Binary");
header("Content-disposition: attachment; filename=\"".basename($file_url)."\"");
readfile($file_url);


?>