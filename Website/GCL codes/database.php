<?php
$process = $_GET['method'];
$username2 = $_GET['username'];
$latitude = $_GET['latitude'];
$longitude = $_GET['longitude'];
$servername = "localhost";
$username = "username";
$password = "password";
$dbname = "id14366874_username";

// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);
// Check connection
if ($conn->connect_error) {
  die("Connection failed: " . $conn->connect_error);
}
if ($process == "create"){
    $sql = "INSERT INTO Userdatabase (username, latitude, longitude)
    VALUES ($username2, $latitude, $longitude)";

    if ($conn->query($sql) === TRUE) {
    echo "New record created successfully";
    }   else {
    echo "Error: " . $sql . "<br>" . $conn->error;
    }

}
else if($process == "update"){
    $sql = "UPDATE Userdatabase SET latitude=$latitude, longitude=$longitude WHERE username=$username2";

    if ($conn->query($sql) === TRUE) {
    echo "Record updated successfully";
    } else {
    echo "Error updating record: " . $conn->error;
    }
}
$conn->close();
?>