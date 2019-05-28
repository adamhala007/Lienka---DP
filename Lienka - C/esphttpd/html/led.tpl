<html><head><title>Test</title>
<link rel="stylesheet" type="text/css" href="style.css">
</head>
<body>
<div id="main">
<h1>The LED</h1>
<p>
If there's a LED connected to GPIO2, it's now %ledstate%. You can change that using the buttons below.
</p>
<form method="post" action="led.cgi">
<input type="submit" name="led" value="2">
<input type="submit" name="led" value="4">
<input type="submit" name="led" value="6">
<input type="submit" name="led" value="8">
</form>
</div>
</body></html>
