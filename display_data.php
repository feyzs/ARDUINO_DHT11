<?php
$servername = "localhost";
$username = "root";
$password = "bulancak";
$dbname = "sensor_data_db";

// mysql bağlantısı
$conn = new mysqli($servername, $username, $password, $dbname);

if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

//POST verilerini al
if ($_SERVER['REQUEST_METHOD'] == 'POST') {
    $temperature = $_POST['temperature'];
    $humidity = $_POST['humidity'];

    // Veritabanına ekle
    $stmt = $conn->prepare("INSERT INTO sensor_data (temperature, humidity) VALUES (?, ?)");
    $stmt->bind_param("dd", $temperature, $humidity);

    if ($stmt->execute()) {
        echo "Veri başarıyla eklendi.";
    } else {
        echo "Veri ekleme hatası: " . $stmt->error;
    }

    $stmt->close();
} else {
   // echo "Veri tabanı bağlantısı başarılı!<br>";

    // Verileri listele
    $sql = "SELECT id, temperature, humidity, timestamp FROM sensor_data ORDER BY timestamp DESC";
    $result = $conn->query($sql);

    if ($result->num_rows > 0) {
        echo "<table><tr><th>ID</th><th>Temperature</th><th>Humidity</th><th>Timestamp</th></tr>";
        while ($row = $result->fetch_assoc()) {
  echo "<tr><td>" . $row["id"]. "&nbsp;&nbsp;&nbsp;&nbsp;" . "</td><td>" . $row["temperature"]. " °C</td><td>" . $row["humidity"]. " %</td><td>" . $row["timestamp"]. "</td></tr>";
        }
        echo "</table>";
    } else {
        echo "0 results";
    }
}

$conn->close();
?>