function initTables() {
    toggleTable(1);
}
function toggleTable(nr) {
    console.log("nr: " + nr);
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

function addToReport(id, url, table_type) {
    console.log(table_type);
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
            console.log(data);
            console.log("btn_" + id);
            $('#btn_' + id).prop('disabled', true);
        });
}

function removeFromReport(id, url) {
    console.log(url);
    var element = document.getElementById("tr_" + id);
    element.outerHTML = "";
    delete element;
    $.post(url,
        {
            object_id: id
        },
        function (data, status) {
            console.log("btn_" + id);
            $('#btn_' + id).prop('disabled', false);
        });
}