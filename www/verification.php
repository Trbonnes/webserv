<?php
session_start();
if(isset($_POST['username']) && isset($_POST['password']))
{

    $_SESSION['username'] = $username;
    header('Location: principale.php');
}
else
{
   header('Location: login.php');
}
?>