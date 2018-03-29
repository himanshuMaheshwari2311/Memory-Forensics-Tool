function initTables() {
    toggleTable(1);
}
function toggleTable(nr) {
    document.getElementById("table1").style.display = "none";
    document.getElementById("table2").style.display = "none";
    document.getElementById("table3").style.display = "none";
    document.getElementById("table4").style.display = "none";
    document.getElementById("table5").style.display = "none";
    document.getElementById("table" + nr).style.display = "table-row";
}
window.onload = initTables;

function addToReport(id, url) {
    console.log(url);
    $.post(url,
        {
            object_id: id
        },
        function (data, status) {
            alert("Data: " + data + "\nStatus: " + status);
        });
}