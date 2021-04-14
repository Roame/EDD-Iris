var timer;
var count = 0;

function startImageFeed(){
    // document.getElementById("imOut").src = "images/".concat(Date().toString());
    if(count >59){
        count = 0;
    }
    var imgDir = "images/test"+count+".jpg";
    console.log("Querying " + imgDir);
    document.getElementById("imOut").src = imgDir;
    // document.getElementById("imOut").src = "images/grandmaNo.jpg"
    count++;
    timer = setTimeout(startImageFeed, 100);
}

function stopFeed(){
    clearTimeout(timer)
}

$(document).ready(function(){
    startImageFeed();
});