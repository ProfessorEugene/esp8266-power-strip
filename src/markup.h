#define CP_HTML "<!DOCTYPE html>\n"\
"<html>\n"\
"\n"\
"<head>\n"\
"    <!-- <meta content='width=device-width; initial-scale=1.0; maximum-scale=1.0; user-scalable=0;' name='viewport' /> -->\n"\
"\n"\
"    <style>\n"\
"        body {\n"\
"            /* background-color: white; */\n"\
"            margin-top: 0px;\n"\
"            margin-bottom: 0px;\n"\
"            margin-left: 5px;\n"\
"            margin-right: 5px;\n"\
"            padding: 1;\n"\
"            overflow: hidden;\n"\
"        }\n"\
"\n"\
"        div.container {\n"\
"            width: 100%;\n"\
"            height: 100%;\n"\
"            border: 1px solid gray;\n"\
"            position: relative;\n"\
"        }\n"\
"\n"\
"        header,\n"\
"        footer {\n"\
"            padding: 1em;\n"\
"            color: white;\n"\
"            background-color: black;\n"\
"            clear: left;\n"\
"            text-align: center;\n"\
"        }\n"\
"\n"\
"        nav {\n"\
"            float: left;\n"\
"            max-width: 160px;\n"\
"            margin: 0;\n"\
"            padding: 1em;\n"\
"        }\n"\
"\n"\
"        nav ul {\n"\
"            list-style-type: none;\n"\
"            padding: 0;\n"\
"        }\n"\
"\n"\
"        nav ul a {\n"\
"            text-decoration: none;\n"\
"        }\n"\
"\n"\
"        article {\n"\
"            margin-left: 170px;\n"\
"            border-left: 1px solid gray;\n"\
"            padding: 1em;\n"\
"            overflow: hidden;\n"\
"            position: relative;\n"\
"            height: 100%;\n"\
"        }\n"\
"\n"\
"        #plugsContainer {\n"\
"            /* width: 90%; */\n"\
"            /* margin-right: 170px; */\n"\
"            width: 100%;\n"\
"            height: 100%;\n"\
"            border: solid black 1px;\n"\
"            position: relative;\n"\
"        }\n"\
"\n"\
"        .plug {\n"\
"            width: 100%;\n"\
"            /* padding: 20px; */\n"\
"            height: 125px;\n"\
"            border: solid black 5px;\n"\
"            background-color: yellow;\n"\
"            position: relative;\n"\
"            box-sizing: border-box;\n"\
"            -moz-box-sizing: border-box;\n"\
"            -webkit-box-sizing: border-box;\n"\
"        }\n"\
"\n"\
"        .on {\n"\
"            background-color: green;\n"\
"        }\n"\
"\n"\
"        .off {\n"\
"            background-color: red;\n"\
"        }\n"\
"    </style>\n"\
"</head>\n"\
"\n"\
"<body>\n"\
"\n"\
"    <div class=\"container\">\n"\
"\n"\
"        <header>\n"\
"            <h1>Strip Control Panel</h1>\n"\
"        </header>\n"\
"\n"\
"        <nav>\n"\
"            <ul>\n"\
"                <li>\n"\
"                    <a href=\"/wifi\">Configure WiFi</a>\n"\
"                </li>\n"\
"                <li>\n"\
"                    <a href=\"https://projects.contextweb.com/erachitskiy/vu/\">Use VU meter</a>\n"\
"                </li>\n"\
"            </ul>\n"\
"        </nav>\n"\
"\n"\
"        <article>\n"\
"            <h1>Strip Status</h1>\n"\
"            <div id=\"plugsContainer\">\n"\
"                <div class=\"plug\">&nbsp;</div>\n"\
"                <div class=\"plug\">&nbsp;</div>\n"\
"                <div class=\"plug\">&nbsp;</div>\n"\
"                <div class=\"plug\">&nbsp;</div>\n"\
"            </div>\n"\
"            <p>Use buttons above to change plug state.</p>\n"\
"\n"\
"        </article>\n"\
"\n"\
"        <footer>Smart Strip v0.0.1</footer>\n"\
"\n"\
"    </div>\n"\
"    <script>\n"\
"        (function () {\n"\
"            var state = 0x0F;\n"\
"            var requesting = false;\n"\
"            var prefix = 'http://strip.local/'\n"\
"            var plugElements = document.getElementsByClassName('plug');\n"\
"            var doGET = function (url, callback) {\n"\
"                requesting = true;\n"\
"                var xhr = new XMLHttpRequest();\n"\
"                xhr.open('GET', url, true);\n"\
"                xhr.onload = function () {\n"\
"                    requesting = false;\n"\
"                    callback(JSON.parse(xhr.responseText));\n"\
"                };\n"\
"                xhr.send(null);\n"\
"            }\n"\
"            var getStatus = function (callback) {\n"\
"                doGET(prefix + 'get', callback);\n"\
"            }\n"\
"            var setStatus = function (status, callback) {\n"\
"                if (requesting == true) {\n"\
"                    return;\n"\
"                }\n"\
"                doGET(prefix + 'set?s=' + status, callback);\n"\
"            }\n"\
"\n"\
"            var computeState = function () {\n"\
"                var state = 0x0F;\n"\
"                for (var i = 0; i < plugElements.length; i++) {\n"\
"                    if (plugElements[i].classList.contains('off')) {\n"\
"                        state = state ^ (0x1 << i);\n"\
"                    }\n"\
"                }\n"\
"                return state;\n"\
"            };\n"\
"            var updateState = function (state) {\n"\
"                for (var i = 0; i < plugElements.length; i++) {\n"\
"                    var st = ((state >> i) & 0x01) == 0x01;\n"\
"                    if (st) {\n"\
"                        /* button is on */\n"\
"                        plugElements[i].className = \"plug on\";\n"\
"                    } else {\n"\
"                        plugElements[i].className = \"plug off\";\n"\
"                    }\n"\
"                }\n"\
"            };\n"\
"            for (var i = 0; i < plugElements.length; i++) {\n"\
"                var plugElement = plugElements[i];\n"\
"                var eventName = ('ontouchstart' in document.documentElement) ? 'touchstart' : 'click';\n"\
"\n"\
"                plugElement.addEventListener(eventName, function () {\n"\
"                    if (this.classList.contains('on')) {\n"\
"                        this.className = \"plug off\";\n"\
"                    } else {\n"\
"                        this.className = \"plug on\";\n"\
"                    }\n"\
"                    setStatus(computeState(), function (res) {\n"\
"                        updateState(res.status);\n"\
"                    });\n"\
"                });\n"\
"\n"\
"\n"\
"            }\n"\
"            var update = function () {\n"\
"                getStatus(function (res) {\n"\
"                    updateState(res.status);\n"\
"                });\n"\
"            }\n"\
"            update();\n"\
"\n"\
"        })();\n"\
"    </script>\n"\
"</body>\n"\
"\n"\
"</html>\n"
