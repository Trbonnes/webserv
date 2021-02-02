<html>
<head>
    <title></title>
</head>
<body>
  <p>
    <?php

    $rest_json = file_get_contents("php://input");

    $username = explode('&', $rest_json);

    $name = explode('=', $username[0])[1];
    
    echo "Hello " . $name;

    ?>
  </p>
</body>