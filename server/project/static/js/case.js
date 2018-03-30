function initTables() {
    toggleTable(1);
}
function toggleTable(nr) {
    try {
        document.getElementById("table1").style.display = "none";
    }
    catch(err) {
        console.log(err);
    }
    try {
        document.getElementById("table2").style.display = "none";
    }
    catch(err) {
        console.log(err);
    }
    try {
        document.getElementById("table3").style.display = "none";
    }
    catch(err) {
        console.log(err);
    }
    try {
        document.getElementById("table4").style.display = "none";
    }
    catch(err) {
        console.log(err);
    }
    try {
        document.getElementById("table5").style.display = "none";
    }
    catch(err) {
        console.log(err);
    }
    try {
        document.getElementById("table6").style.display = "none";
    }
    catch(err) {
        console.log(err);
    }
    try {
        document.getElementById("table77").style.display = "none";
    }
    catch(err) {
        console.log(err);
    }
    document.getElementById("table" + nr).style.display = "table-row";
}
window.onload = initTables;

function addToReport(id, url) {
    var comment_msg = prompt("Add some comment regarding this Artifact");
    if (comment_msg == null || comment_msg == "") {
        txt = " ";
    } 
    console.log(url);
    $.post(url,
        {
            object_id: id,
            comment: comment_msg
        },
        function (data, status) {
            console.log("btn_" + id);
            $('#btn_' + id).prop('disabled', true);
        });
}

function removeFromReport(id, url) {
    var comment_msg = prompt("Add some comment regarding this Artifact");
    if (comment_msg == null || comment_msg == "") {
        txt = " ";
    } 
    console.log(url);
    $.post(url,
        {
            object_id: id,
            comment: comment_msg
        },
        function (data, status) {
            console.log("btn_" + id);
            $('#btn_' + id).prop('disabled', false);
        });
}