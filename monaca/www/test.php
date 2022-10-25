<?php
if (isset($_POST["submit"])) {
    print($_POST["name"]);
    print($_POST["number"]);
} else {
    print("fail");
}
