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
    label1.innerHTML = "LED Brightness - " + input1.value;

    input1.oninput = function(){
        label1.innerHTML = "LED Brightness - " + this.value;
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

function triggerCrop(){
    let form = $("#picForm");

    $.ajax({
        type: "post",
        url: "triggerCrop.php",
        data: form.serialize(),
        xhrFields: {
            responseType: 'blob' // to avoid binary data being mangled on charset conversion
        },
        success: function(blob, status, xhr) {
            // check for a filename
            var filename = "";
            var disposition = xhr.getResponseHeader('Content-Disposition');
            if (disposition && disposition.indexOf('attachment') !== -1) {
                var filenameRegex = /filename[^;=\n]*=((['"]).*?\2|[^;\n]*)/;
                var matches = filenameRegex.exec(disposition);
                if (matches != null && matches[1]) filename = matches[1].replace(/['"]/g, '');
            }
    
            if (typeof window.navigator.msSaveBlob !== 'undefined') {
                // IE workaround for "HTML7007: One or more blob URLs were revoked by closing the blob for which they were created. These URLs will no longer resolve as the data backing the URL has been freed."
                window.navigator.msSaveBlob(blob, filename);
            } else {
                var URL = window.URL || window.webkitURL;
                var downloadUrl = URL.createObjectURL(blob);
    
                if (filename) {
                    // use HTML5 a[download] attribute to specify filename
                    var a = document.createElement("a");
                    // safari doesn't support this yet
                    if (typeof a.download === 'undefined') {
                        window.location.href = downloadUrl;
                    } else {
                        a.href = downloadUrl;
                        a.download = filename;
                        document.body.appendChild(a);
                        a.click();
                    }
                } else {
                    window.location.href = downloadUrl;
                }
    
                setTimeout(function () { URL.revokeObjectURL(downloadUrl); }, 100); // cleanup
            }
        }
    });
    console.log("Ran");
    return false;
}

$(document).ready(function(){
    initUI();
    startImageFeed();
    sendConfig();
});