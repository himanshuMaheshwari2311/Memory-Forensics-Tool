function initTables() {
    toggleTable(1);
}
function toggleTable(nr) {
    console.log("nr: " + nr);
    try {
        document.getElementById("table1").style.display = "none";
    }
    catch (err) {
        console.log(err);
    }
    try {
        document.getElementById("table2").style.display = "none";
    }
    catch (err) {
        console.log(err);
    }
    try {
        document.getElementById("table3").style.display = "none";
    }
    catch (err) {
        console.log(err);
    }
    try {
        document.getElementById("table4").style.display = "none";
    }
    catch (err) {
        console.log(err);
    }
    try {
        document.getElementById("table5").style.display = "none";
    }
    catch (err) {
        console.log(err);
    }
    try {
        document.getElementById("table6").style.display = "none";
    }
    catch (err) {
        console.log(err);
    }
    try {
        document.getElementById("table7").style.display = "none";
    }
    catch (err) {
        console.log(err);
    }
    try {
        document.getElementById("table8").style.display = "none";
    }
    catch (err) {
        console.log(err);
    }
    try {
        document.getElementById("table77").style.display = "none";
    }
    catch (err) {
        console.log(err);
    }
    document.getElementById("table" + nr).style.display = "table-row";
}
window.onload = initTables;

function addToReport(id, url, table_type, url2) {

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
            console.log(data, status);
            if (status == "success") {
                console.log("btn_" + id);
                $('#btn_' + id).prop('disabled', true);
                try {
                    document.getElementById(table_type + "_tr").style.display = "none";
                }
                catch (err) {
                    console.log(err);
                }
                data = JSON.parse(data);
                artifact = data["obj"]
                console.log(artifact);

                var table = document.getElementById(table_type).getElementsByTagName('tbody')[0];
                var row = table.insertRow(table.rows.length);
                var att = document.createAttribute("id");
                att.value = "tr_" + id;
                row.setAttributeNode(att);
                var i = 0;
                for (var key in artifact) {
                    if (key != "marked" && key != "start" && key != "object_id") {
                        console.log(artifact[key]);
                        if (key == "phyical_offset")
                            row.insertCell(i).innerHTML = artifact[key].toString(16);
                        else row.insertCell(i).innerHTML = artifact[key];
                        i++;
                    }
                }
                row.insertCell(i).innerHTML = '<button type="button" class="btn btn-primary" id="btn_d_{{id[0]}}" onclick="removeFromReport(\'' + id + '\', \'' + url2 + '\')" enabled>Remove</button>';
            }
            else {
                alert("Failed to Add");
            }
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
            if (status == "success") {
                $('#btn_' + id).prop('disabled', false);
            }
            else {
                alert("Failed to remove");
            }
        });
}

function updateReport(id, url) {
    console.log(url);
    var overview = $('#overview').val();
    var acquisition = $('#acquisition').val();
    var findings = $('#findings').val();
    var conclusion = $('#conclusion').val();
    console.log(overview);
    $.post(url,
        {
            case_overview: overview,
            case_acquisition: acquisition,
            case_findings: findings,
            case_conclusion: conclusion
        },
        function (data, status) {
            console.log(data);
            if (status == "success") {
                alert("Updated Successfully!");
            }
            else {
                alert("Updated Failed");
            }
        });
}