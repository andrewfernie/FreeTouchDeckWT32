
function createCheck(filename) {
    return `<input form="delete" type="checkbox" id="${filename}" name="${filename}" value="${filename}"><label for="${filename}"> ${filename}</label><br>`;
}

function fillvaluelist(targetNode, srcArray) {

    targetNode.innerHTML = srcArray.reduce((options, { value, name }) =>
        options += `<option value="${value}">${name}</option>`,
        '');

}

function updateMenuItems(select) {
    var selectedOption = select.options[select.selectedIndex];
//    alert("The selected menu is " + selectedOption.value);
    getMenuConfig(selectedOption.value);
}

function getMenuConfig(loadingMenuNumber) {

    menuFileName = `config/menu${loadingMenuNumber}.json`;
    console.log('getMenuConfig(): called with menu number ' + loadingMenuNumber);

    // Load Menu
    fetch(menuFileName)
        .then((response) => {
            return response.json()
        })
        .then((data) => {
            console.log('getMenuConfig(): loading menu ' + menuFileName);

            ////////////////////////// Row 1 ///////////////////////////
            document.getElementById('button11logo').value = data.button11.logo;

            if (data.button11.latch) {
                document.getElementById("button11latch").checked = true;
            }

            if (data.button11.latchlogo == "") {
                document.getElementById("button11latchlogo").value = "---";
            } else {
                document.getElementById("button11latchlogo").value = data.button11.latchlogo;
            }

            document.getElementById('button11action0').value = data.button11.actionarray[0];
            document.getElementById('button11action1').value = data.button11.actionarray[1];
            document.getElementById('button11action2').value = data.button11.actionarray[2];
            fillvaluelist(button11value0, selecteditems[data.button11.actionarray[0]].subitems);
            document.getElementById('button11value0').value = data.button11.valuearray[0];
            fillvaluelist(button11value1, selecteditems[data.button11.actionarray[1]].subitems);
            document.getElementById('button11value1').value = data.button11.valuearray[1];
            fillvaluelist(button11value2, selecteditems[data.button11.actionarray[2]].subitems);
            document.getElementById('button11value2').value = data.button11.valuearray[2];


            document.getElementById("button12logo").value = data.button12.logo;
            if (data.button12.latch) {
                document.getElementById("button12latch").checked = true;
            }

            if (data.button12.latchlogo == "") {
                document.getElementById("button12latchlogo").value = "---";
            } else {
                document.getElementById("button12latchlogo").value = data.button12.latchlogo;
            }


            document.getElementById('button12action0').value = data.button12.actionarray[0];
            document.getElementById('button12action1').value = data.button12.actionarray[1];
            document.getElementById('button12action2').value = data.button12.actionarray[2];
            fillvaluelist(button12value0, selecteditems[data.button12.actionarray[0]].subitems);
            document.getElementById('button12value0').value = data.button12.valuearray[0];
            fillvaluelist(button12value1, selecteditems[data.button12.actionarray[1]].subitems);
            document.getElementById('button12value1').value = data.button12.valuearray[1];
            fillvaluelist(button12value2, selecteditems[data.button12.actionarray[2]].subitems);
            document.getElementById('button12value2').value = data.button12.valuearray[2];

            document.getElementById("button13logo").value = data.button13.logo;
            if (data.button13.latch) {
                document.getElementById("button13latch").checked = true;
            }

            if (data.button13.latchlogo == "") {
                document.getElementById("button13latchlogo").value = "---";
            } else {
                document.getElementById("button13latchlogo").value = data.button13.latchlogo;
            }

            document.getElementById('button13action0').value = data.button13.actionarray[0];
            document.getElementById('button13action1').value = data.button13.actionarray[1];
            document.getElementById('button13action2').value = data.button13.actionarray[2];
            fillvaluelist(button13value0, selecteditems[data.button13.actionarray[0]].subitems);
            document.getElementById('button13value0').value = data.button13.valuearray[0];
            fillvaluelist(button13value1, selecteditems[data.button13.actionarray[1]].subitems);
            document.getElementById('button13value1').value = data.button13.valuearray[1];
            fillvaluelist(button13value2, selecteditems[data.button13.actionarray[2]].subitems);
            document.getElementById('button13value2').value = data.button13.valuearray[2];

            document.getElementById("button14logo").value = data.button14.logo;
            if (data.button14.latch) {
                document.getElementById("button14latch").checked = true;
            }

            if (data.button14.latchlogo == "") {
                document.getElementById("button14latchlogo").value = "---";
            } else {
                document.getElementById("button14latchlogo").value = data.button14.latchlogo;
            }

            document.getElementById('button14action0').value = data.button14.actionarray[0];
            document.getElementById('button14action1').value = data.button14.actionarray[1];
            document.getElementById('button14action2').value = data.button14.actionarray[2];
            fillvaluelist(button14value0, selecteditems[data.button14.actionarray[0]].subitems);
            document.getElementById('button14value0').value = data.button14.valuearray[0];
            fillvaluelist(button14value1, selecteditems[data.button14.actionarray[1]].subitems);
            document.getElementById('button14value1').value = data.button14.valuearray[1];
            fillvaluelist(button14value2, selecteditems[data.button14.actionarray[2]].subitems);
            document.getElementById('button14value2').value = data.button14.valuearray[2];

            ////////////////////////// Row 2 ///////////////////////////
            document.getElementById("button21logo").value = data.button21.logo;

            if (data.button21.latch) {
                document.getElementById("button21latch").checked = true;
            }

            if (data.button21.latchlogo == "") {
                document.getElementById("button21latchlogo").value = "---";
            } else {
                document.getElementById("button21latchlogo").value = data.button21.latchlogo;
            }

            document.getElementById('button21action0').value = data.button21.actionarray[0];
            document.getElementById('button21action1').value = data.button21.actionarray[1];
            document.getElementById('button21action2').value = data.button21.actionarray[2];
            fillvaluelist(button21value0, selecteditems[data.button21.actionarray[0]].subitems);
            document.getElementById('button21value0').value = data.button21.valuearray[0];
            fillvaluelist(button21value1, selecteditems[data.button21.actionarray[1]].subitems);
            document.getElementById('button21value1').value = data.button21.valuearray[1];
            fillvaluelist(button21value2, selecteditems[data.button21.actionarray[2]].subitems);
            document.getElementById('button21value2').value = data.button21.valuearray[2];


            document.getElementById("button22logo").value = data.button22.logo;
            if (data.button22.latch) {
                document.getElementById("button22latch").checked = true;
            }

            if (data.button22.latchlogo == "") {
                document.getElementById("button22latchlogo").value = "---";
            } else {
                document.getElementById("button22latchlogo").value = data.button22.latchlogo;
            }


            document.getElementById('button22action0').value = data.button22.actionarray[0];
            document.getElementById('button22action1').value = data.button22.actionarray[1];
            document.getElementById('button22action2').value = data.button22.actionarray[2];
            fillvaluelist(button22value0, selecteditems[data.button22.actionarray[0]].subitems);
            document.getElementById('button22value0').value = data.button22.valuearray[0];
            fillvaluelist(button22value1, selecteditems[data.button22.actionarray[1]].subitems);
            document.getElementById('button22value1').value = data.button22.valuearray[1];
            fillvaluelist(button22value2, selecteditems[data.button22.actionarray[2]].subitems);
            document.getElementById('button22value2').value = data.button22.valuearray[2];

            document.getElementById("button23logo").value = data.button23.logo;
            if (data.button23.latch) {
                document.getElementById("button23latch").checked = true;
            }

            if (data.button23.latchlogo == "") {
                document.getElementById("button23latchlogo").value = "---";
            } else {
                document.getElementById("button23latchlogo").value = data.button23.latchlogo;
            }

            document.getElementById('button23action0').value = data.button23.actionarray[0];
            document.getElementById('button23action1').value = data.button23.actionarray[1];
            document.getElementById('button23action2').value = data.button23.actionarray[2];
            fillvaluelist(button23value0, selecteditems[data.button23.actionarray[0]].subitems);
            document.getElementById('button23value0').value = data.button23.valuearray[0];
            fillvaluelist(button23value1, selecteditems[data.button23.actionarray[1]].subitems);
            document.getElementById('button23value1').value = data.button23.valuearray[1];
            fillvaluelist(button23value2, selecteditems[data.button23.actionarray[2]].subitems);
            document.getElementById('button23value2').value = data.button23.valuearray[2];

            document.getElementById("button24logo").value = data.button24.logo;
            if (data.button24.latch) {
                document.getElementById("button24latch").checked = true;
            }

            if (data.button24.latchlogo == "") {
                document.getElementById("button24latchlogo").value = "---";
            } else {
                document.getElementById("button24latchlogo").value = data.button24.latchlogo;
            }

            document.getElementById('button24action0').value = data.button24.actionarray[0];
            document.getElementById('button24action1').value = data.button24.actionarray[1];
            document.getElementById('button24action2').value = data.button24.actionarray[2];
            fillvaluelist(button24value0, selecteditems[data.button24.actionarray[0]].subitems);
            document.getElementById('button24value0').value = data.button24.valuearray[0];
            fillvaluelist(button24value1, selecteditems[data.button24.actionarray[1]].subitems);
            document.getElementById('button24value1').value = data.button24.valuearray[1];
            fillvaluelist(button24value2, selecteditems[data.button24.actionarray[2]].subitems);
            document.getElementById('button24value2').value = data.button24.valuearray[2];           

            ////////////////////////// Row 3 ///////////////////////////
            document.getElementById("button31logo").value = data.button31.logo;

            if (data.button31.latch) {
                document.getElementById("button31latch").checked = true;
            }

            if (data.button31.latchlogo == "") {
                document.getElementById("button31latchlogo").value = "---";
            } else {
                document.getElementById("button31latchlogo").value = data.button31.latchlogo;
            }

            document.getElementById('button31action0').value = data.button31.actionarray[0];
            document.getElementById('button31action1').value = data.button31.actionarray[1];
            document.getElementById('button31action2').value = data.button31.actionarray[2];
            fillvaluelist(button31value0, selecteditems[data.button31.actionarray[0]].subitems);
            document.getElementById('button31value0').value = data.button31.valuearray[0];
            fillvaluelist(button31value1, selecteditems[data.button31.actionarray[1]].subitems);
            document.getElementById('button31value1').value = data.button31.valuearray[1];
            fillvaluelist(button31value2, selecteditems[data.button31.actionarray[2]].subitems);
            document.getElementById('button31value2').value = data.button31.valuearray[2];


            document.getElementById("button32logo").value = data.button32.logo;
            if (data.button32.latch) {
                document.getElementById("button32latch").checked = true;
            }

            if (data.button32.latchlogo == "") {
                document.getElementById("button32latchlogo").value = "---";
            } else {
                document.getElementById("button32latchlogo").value = data.button32.latchlogo;
            }


            document.getElementById('button32action0').value = data.button32.actionarray[0];
            document.getElementById('button32action1').value = data.button32.actionarray[1];
            document.getElementById('button32action2').value = data.button32.actionarray[2];
            fillvaluelist(button32value0, selecteditems[data.button32.actionarray[0]].subitems);
            document.getElementById('button32value0').value = data.button32.valuearray[0];
            fillvaluelist(button32value1, selecteditems[data.button32.actionarray[1]].subitems);
            document.getElementById('button32value1').value = data.button32.valuearray[1];
            fillvaluelist(button32value2, selecteditems[data.button32.actionarray[2]].subitems);
            document.getElementById('button32value2').value = data.button32.valuearray[2];

            document.getElementById("button33logo").value = data.button33.logo;
            if (data.button33.latch) {
                document.getElementById("button33latch").checked = true;
            }

            if (data.button33.latchlogo == "") {
                document.getElementById("button33latchlogo").value = "---";
            } else {
                document.getElementById("button33latchlogo").value = data.button33.latchlogo;
            }

            document.getElementById('button33action0').value = data.button33.actionarray[0];
            document.getElementById('button33action1').value = data.button33.actionarray[1];
            document.getElementById('button33action2').value = data.button33.actionarray[2];
            fillvaluelist(button33value0, selecteditems[data.button33.actionarray[0]].subitems);
            document.getElementById('button33value0').value = data.button33.valuearray[0];
            fillvaluelist(button33value1, selecteditems[data.button33.actionarray[1]].subitems);
            document.getElementById('button33value1').value = data.button33.valuearray[1];
            fillvaluelist(button33value2, selecteditems[data.button33.actionarray[2]].subitems);
            document.getElementById('button33value2').value = data.button33.valuearray[2];

            document.getElementById("button34logo").value = data.button34.logo;
            if (data.button34.latch) {
                document.getElementById("button34latch").checked = true;
            }

            if (data.button34.latchlogo == "") {
                document.getElementById("button34latchlogo").value = "---";
            } else {
                document.getElementById("button34latchlogo").value = data.button34.latchlogo;
            }

            document.getElementById('button34action0').value = data.button34.actionarray[0];
            document.getElementById('button34action1').value = data.button34.actionarray[1];
            document.getElementById('button34action2').value = data.button34.actionarray[2];
            fillvaluelist(button34value0, selecteditems[data.button34.actionarray[0]].subitems);
            document.getElementById('button34value0').value = data.button34.valuearray[0];
            fillvaluelist(button34value1, selecteditems[data.button34.actionarray[1]].subitems);
            document.getElementById('button34value1').value = data.button34.valuearray[1];
            fillvaluelist(button34value2, selecteditems[data.button34.actionarray[2]].subitems);
            document.getElementById('button34value2').value = data.button34.valuearray[2];

        })
        .catch((err) => {
            console.log(err)
        })
}


function getlogoconfig() {

    var logolist = []; // Array holding the logos (empty to start)

    var selects = document.getElementsByClassName('images');

    fetch('/list?dir=/logos')
        .then((response3) => {
            return response3.json()
        })
        .then((data3) => {
            var checkboxlist = '<form method="post" id="delete" action="/editor">';
            data3.forEach(obj => {
                Object.entries(obj).forEach(([key, value]) => {

                    var input = createCheck(value); // Creates the html for the checkbox
                    checkboxlist += input;

                });

            });
            checkboxlist += '<br><button style="cursor: pointer;"" type="save">Delete files</button></form>';
            document.getElementById("deletefilelist").innerHTML = checkboxlist;

        })
        .catch((err) => {
            console.log(err)
        })

        //        })
        .then((data) => {
            fetch('/info')
                .then((response4) => {
                    return response4.json()
                })
                .then((data4) => {
                    var divcontent = '';
                    data4.forEach(obj => {
                        Object.entries(obj).forEach(([key, value]) => {

                            divcontent += `${key}: ${value}<br>`;
                        });

                    });
                    document.getElementById("infocontent").innerHTML = divcontent;

                })
                .catch((err) => {
                    console.log(err)
                })

        })

        .then((data) => {
            fetch('/list?dir=/logos')
                .then((response2) => {
                    return response2.json()
                })
                .then((data2) => {
                    data2.forEach(obj => {
                        Object.entries(obj).forEach(([key, value]) => {
                            logolist.push(`${value}`);
                            var i = selects.length;
                            while (i--) {
                                var select = selects[i];
                                select.add(new Option(`${value}`, `${value}`));
                            }

                        });
                    });

                    var i = selects.length;
                    while (i--) {
                        var select = selects[i];
                        select.add(new Option('---', '---'));
                    }
                })
                .then(() => {
                    document.getElementById("contentloading").style.display = "none";
                    document.getElementById('ball-loader').style.display = "none";
                    document.getElementById("maincontent").style.display = "block";
                })
                .catch((err) => {
                    console.log(err)
                })

        })
        .catch((err) => {
            console.log(err)
        })


}

// Function to get Sleep settings:
function getwificonfig() {

    // Load Menu 5
    fetch('config/wificonfig.json')
        .then((response) => {
            return response.json()
        })
        .then((data) => {


            document.getElementById("ssid").value = data.ssid;
            document.getElementById("password").value = data.password;
            document.getElementById("wifimode").value = data.wifimode;
            document.getElementById("wifihostname").value = data.wifihostname;
            document.getElementById("attempts").value = data.attempts;
            document.getElementById("attemptdelay").value = data.attemptdelay;


        })
        .catch((err) => {
            console.log(err)
        })
}

// Load Colors and then other configs
fetch('config/general.json')
    .then((response) => {
        return response.json()
    })
    .then((data) => {

        document.getElementById("menubuttoncolor").value = data.menubuttoncolor;
        document.getElementById("functionbuttoncolor").value = data.functionbuttoncolor;
        document.getElementById("latchcolor").value = data.latchcolor;
        document.getElementById("background").value = data.background;
        document.getElementById("sleepenable").value = data.sleepenable;
        document.getElementById("sleeptimer").value = data.sleeptimer;
        document.getElementById("beep").value = data.beep;
        document.getElementById("modifier1").value = data.modifier1;
        document.getElementById("modifier2").value = data.modifier2;
        document.getElementById("modifier3").value = data.modifier3;
        document.getElementById("helperdelay").value = data.helperdelay;

    }).then(() => {
        getwificonfig();
    })
    .then(() => {
        getMenuConfig(0);
    })
    .then(() => {
        getlogoconfig();
    })
    .catch((err) => {
        console.log(err)
    })
