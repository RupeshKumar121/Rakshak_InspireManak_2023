

<?php
$username2 = $_GET['username'];
$latitude = $_GET['latitude'];
$longitude = $_GET['longitude'];
$servername = "localhost";
$username = "username";
$password = "password";
$dbname = "dbname";

/* Attempt MySQL server connection. Assuming you are running MySQL
server with default setting (user 'root' with no password) */
$link = mysqli_connect($servername, $username, $password, $dbname);
 
// Check connection
if($link === false){
    die("ERROR: Could not connect. " . mysqli_connect_error());
}

$sql = "SELECT * FROM Userdatabase WHERE username !=$username2 AND 1>=(twopoints_on_earth($latitude, $longitude,latitude, longitude))";
if($result = mysqli_query($link, $sql)){
    if(mysqli_num_rows($result) > 0){
        $i = 0;
        echo "NULL_";
        while($row = mysqli_fetch_array($result)){
            $i = $i + 1;
            if ($i < mysqli_num_rows($result)){
                echo $row['username'];
                echo "_";
            }
            else{
                echo $row['username'];
            }
            /*echo $row['username'];
            echo "_";
            */    
                
               
            
            
        }
        // Close result set
        mysqli_free_result($result);
    } else{
        echo "";
    }
} else{
    echo "ERROR: Could not able to execute $sql. " . mysqli_error($link);
}
 
// Close connection
mysqli_close($link);
?>
