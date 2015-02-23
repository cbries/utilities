<?php
/*
 * Copyright (c) 2009 Christian Benjamin Ries
 * Website: https://github.com/cbries/utilities
 * License: MIT
 */
?>
<br>
<h3>Add...</h3>
<form method="post">
<input type="hidden" name="gbaction" value="add">
Username: <input type="text" name="frm[username]" value="<?php echo($_SESSION['frm']['username']);?>"><br>
E-Mail: <input type="text" name="frm[mail]" value="<?php echo($_SESSION['frm']['mail']);?>"><br>
Subject: <input type="text" name="frm[subject]" value="<?php echo($_SESSION['frm']['subject']);?>"><br>
Message: <input type="text" name="frm[message]" value="<?php echo($_SESSION['frm']['message']);?>"><br>
Captcha: <img src="showCaptcha.php"> <input type="text" name="frm[captchares]"><br>
<input type="submit" value="Store">
</form>
<br><br>
