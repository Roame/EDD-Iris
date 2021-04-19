var timer;
var count = 0;

function startImageFeed(){
    var imgDir = "images/feed.jpg?t=" + new Date().getTime();
    document.getElementById("imOut").src = imgDir;
    timer = setTimeout(startImageFeed, 200);
}

function stopFeed(){
    clearTimeout(timer)
}

$(document).ready(function(){
    startImageFeed();
});