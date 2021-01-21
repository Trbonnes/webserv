<html>
    <head>
        <meta charset="utf-8">
        <!-- importer le fichier de style -->
        <link rel="stylesheet" href="style.css" media="screen" type="text/css" />
    </head>
    <body style='background:#fff;'>
        <div id="content">
            <!-- tester si l'utilisateur est connecté -->
            <?php
                // isset($_POST['username']);
                // isset($_POST['password']);
                $_SESSION['username'] = $username;
                echo $username;
                echo "Bonjour" . $username . ", vous êtes connecté";
            ?>
            
        </div>
    </body>
</html>
