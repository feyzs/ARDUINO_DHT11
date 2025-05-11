<?php

ini_set('display_errors',1);
ini_set('display_startup_errors',1);
error_reporting(E_ALL);


$servername = "localhost";
$username = "root";
$password = "bulancak";
$dbname = "sensor_data_db";

$conn = new mysqli($servername, $username, $password, $dbname);

if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}
// Sadece son 1 haftanın verilerini her saat başı alacak şekilde sorgu
$sql = "SELECT MAX(temperature) AS temperature, MAX(humidity) AS humidity, 
        MAX(timestamp) AS timestamp 
        FROM sensor_data 
        WHERE timestamp >= NOW() - INTERVAL 1 WEEK 
        GROUP BY UNIX_TIMESTAMP(timestamp) DIV 3600";
$result = $conn->query($sql);

$temperatures = [];
$timestamps = [];
$humidities = [];

if ($result->num_rows > 0) {
    while($row = $result->fetch_assoc()) {
        $temperatures[] = $row["temperature"];
        $humidities[] = $row["humidity"];
        $timestamps[] = $row["timestamp"];
    }
} else {
    $temperatures = ["N/A"];
    $humidities = ["N/A"];
    $timestamps = ["N/A"];
}

$conn->close();
?>

<!DOCTYPE HTML><html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
  <style>
    html {
     font-family: Arial;
     display: inline-block;
     margin: 0px auto;
     text-align: center;
    }
    .chart-container {
      width: 650px;
      height: 400px;
      margin: 0 auto;
    }
  </style>
</head>
<body>
  <h2>TEMPERATURE AND HUMIDITY GRAPHS</h2>
  <div class="chart-container">
    <canvas id="temperatureChart"></canvas>
    <canvas id="humidityChart"></canvas>
  </div>
  <script>
    var tempLabels = <?php echo json_encode(array_reverse($timestamps)); ?>;
    var tempData = <?php echo json_encode(array_reverse($temperatures)); ?>;
    var humidityData = <?php echo json_encode(array_reverse($humidities)); ?>;
    
    var ctx = document.getElementById('temperatureChart').getContext('2d');
    var temperatureChart = new Chart(ctx, {
        type: 'line',
        data: {
            labels: tempLabels,
            datasets: [{
                label: 'Temperature (°C)',
                backgroundColor: 'rgb(255, 99, 132)',
                borderColor: 'rgb(255, 99, 132)',
                data: tempData,
                fill: false,
            }]
        },
        options: {
            scales: {
                xAxes: [{
                    type: 'time',
                    time: {
                        unit: 'minute',
                        stepSize: 10,
                        displayFormats: {
                            minute: 'HH:mm'
                        }
                    }
                }],
                yAxes: [{
                    ticks: {
                        beginAtZero: true
                    }
                }]
            }
        }
    });

    var ctxH = document.getElementById('humidityChart').getContext('2d');
    var humidityChart = new Chart(ctxH, {
        type: 'line',
        data: {
            labels: tempLabels,
            datasets: [{
                label: 'Humidity (%)',
                backgroundColor: 'rgb(54, 162, 235)',
                borderColor: 'rgb(54, 162, 235)',
                data: humidityData,
                fill: false,
            }]
        },
        options: {
            scales: {
                xAxes: [{
                    type: 'time',
                    time: {
                        unit: 'minute',
                        stepSize: 10,
                        displayFormats: {
                            minute: 'HH:mm'
                        }
                    }
                }],
                yAxes: [{
                    ticks: {
                        beginAtZero: true
                    }
                }]
            }
        }
    });
  </script>
</body>
</html>