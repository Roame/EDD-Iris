var timer;
var count = 0;
var cfgSubTimer;

function startImageFeed(){
    var imgDir = "images/feed.jpg?t=" + new Date().getTime();
    document.getElementById("videoFeed").src = imgDir;
    timer = setTimeout(startImageFeed, 200);
}

function stopFeed(){
    clearTimeout(timer)
}

function initUI(){
    var input1 = document.getElementById("input1");
    var label1 = document.getElementById("label1");
    label1.innerHTML = "Input 1 - " + input1.value;

    input1.oninput = function(){
        label1.innerHTML = "Input 1 - " + this.value;
    }

    var input2 = document.getElementById("input2");
    var label2 = document.getElementById("label2");
    label2.innerHTML = "Input 2 - " + input2.value;

    input2.oninput = function(){
        label2.innerHTML = "Input 2 - " + this.value;
    }
}

function sendConfig(){
    let form = $("#configForm");

    $.ajax({
        type: "post",
        url: "writeConfig.php",
        data: form.serialize(),
        success: function(response){
            console.log(response);
        }
    });

    cfgSubTimer = setTimeout(sendConfig, 333);
}

$(document).ready(function(){
    initUI();
    startImageFeed();
    // sendConfig();
});