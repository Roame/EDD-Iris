var timer;

function startImageFeed(){
    var imgDir = "../images/feed.jpg?t=" + new Date().getTime();
    document.getElementById("fsImg").src = imgDir;
    timer = setTimeout(startImageFeed, 200);
}

$(document).ready(function(){
    startImageFeed();
});