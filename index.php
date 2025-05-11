<?php
$servername = "localhost";
$username = "root";
$password = "bulancak";
$dbname = "sensor_data_db";

$conn = new mysqli($servername, $username, $password, $dbname);

if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

$sql = "SELECT temperature, humidity, timestamp FROM sensor_data ORDER BY timestamp DESC LIMIT 1";
$result = $conn->query($sql);

if ($result->num_rows > 0) {
    $row = $result->fetch_assoc();
    $temperature = $row["temperature"];
    $humidity = $row["humidity"];
    $timestamp = $row["timestamp"];
} else {
    $temperature = "N/A";
    $humidity = "N/A";
    $timestamp = "N/A";
}

$conn->close();
?>

<!DOCTYPE HTML><html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.0.0-beta3/css/all>

  <style>
    html {
     font-family: Arial;
     display: inline-block;
     margin: 0px auto;
     text-align: center;
    }
    h2 { font-size: 3rem; }
    p { font-size: 2rem; }
    .units { font-size: 2rem; }
    .dht-labels{
      font-size: 2rem;
      vertical-align:10px;
      padding-bottom: 15px;
    }
  </style>
</head>
<body>
  <h2>ESP8226 DHT 11 IoT SERVER</h2>
  <p>
    <i class="fas fa-sun" style="color:#db3716;font-size: 3.5rem;"></i>
    <span class="dht-labels">Temperature</span> <br> <br>
    <span id="temperature"> <mark><?php echo $temperature; ?> </mark> </span>
    <span class="units">°C</span>
  </p>
  <br><br><br>
  <p>
    <i class="fas fa-cloud" style="color:#4dc0db;font-size: 3.5rem;"></i>
    <span class="dht-labels">Humidity</span> <br><br>
    <span id="humidity"> <mark><?php echo $humidity; ?> </mark> </span>
    <span class="units">%</span>
  </p>
  <p>Last updated: <?php echo $timestamp; ?></p>
</body>
</html>