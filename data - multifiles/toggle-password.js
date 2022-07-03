function togglepassword() {
    var field = document.getElementById("password");
    if (field.type === "password") {
        field.type = "text";
    } else {
        field.type = "password";
    }
}