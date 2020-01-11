<?php

	// Sends html mail with attachment (optional) to one or more recipients
	//
	// Input:
	// to_addr:				one or more email addresses seperated by a ','
	// from_name:			name of the sender. e.g.: John Doe
	// from_addr:			email address of the sender (for reply to's)
	// subject:				the subject line of the message
	// message:				message contents
	// attachment:		file name of the attachment (only jpeg!)

	require('mime_mail.class'); //add mail engine

	//set attachment
	if (isset($attachment)) {
		$fp = fopen($attachment, 'r')
			or die ("unable to open file ($attachment)");
		$filename = fread($fp, filesize($attachment));
		fclose($fp);
		//set att. name
		$attachment = substr($attachment, strrpos($attachment, "/")+1);
	}

	//create the mail object.
	$mail = new html_mime_mail('X-Mailer: PHPXMail');

	//create content
	$html = "<html><body>\r\n" . $message . "<br>\r\n</body></html>";
	$mail->add_html($html, $message);
	if (isset($attachment)) {
		$mail->add_attachment($filename, $attachment, 'image/jpg');
	}
	$mail->set_charset('iso-8859-1', TRUE);

	//build message
	$mail->build_message();

	//send
	if (!isset($from_name) || $from_name == '') {
		$from_name = substr($from_addr, 0, strpos($from_addr, "@"));
	}
	//$mail->send('TO NAME', 'TO ADDRESS', 'FROM NAME', 'FROM ADDRESS', 'SUBJECT LINE');
	$mail->send($to_addr, $to_addr, $from_name, $from_addr, stripslashes($subject))
		or die('error sending mail');

?>