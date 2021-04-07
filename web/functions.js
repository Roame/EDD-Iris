var timer;
var toggle = false;

function startImageFeed(){
    // document.getElementById("imOut").src = "images/".concat(Date().toString());
    if(toggle){
        document.getElementById("imOut").src = "images/grandmaNo.jpg";
    } else {
        document.getElementById("imOut").src = "images/patriarchy.jpg";
    }
    toggle = !toggle
    timer = setTimeout(startImageFeed, 67);
}

function stopFeed(){
    clearTimeout(timer)
}

$(document).ready(function(){
    startImageFeed();
});