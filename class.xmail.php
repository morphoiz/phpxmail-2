<?php
/*

No Copyright (nC) 2001,2002 SOGware by Paul Heijman.
Released under the GPL and PHP licenses as stated in the the README file which
should have been included with this document.

*/

//////////////
//// General functions

// Encrypts password
function xmcrypt ($password) {
	$crypt = "";
	for ($i = 0; $i < strlen($password); $i++) {
		$byte = dechex((ord(substr($password, $i, 1)) ^ 101) & 255);
		$byte = str_pad($byte, 2, '0', STR_PAD_LEFT);
		$crypt .= $byte;
	}
	return strtolower($crypt);
} //end xmcrypt

// Decrypts password
function xmdecrypt ($crypt) {
	$password = "";
	for ($i = 0; $i < strlen($crypt); $i += 2) {
		$byte = hexdec(substr($crypt, $i, 2)) ^ 101;
		$password .= chr($byte);
	}
	return $password;
} //end xmdecrypt

// returns human readable size
function humansize($bytes) {
	$suffix = Array(' b', ' Kb', ' Mb', ' Gb', ' Tb');
	if ($bytes == 0) {
		return '(empty)';
	} else {
		$i = 0;        $b = $bytes;
		while ($b >= 1024) {$i++; $b = $b / 1024;}
	}
	if ($b<1 || $i==0) {
		if ($bytes < 1000) {
			return  $bytes .$suffix[$i];
		} else {
			return  sprintf("%01.2f", $b) .$suffix[$i];
		}
	} elseif ($b < 10) {
		return  sprintf("%01.2f", $b) .$suffix[$i];
	} elseif ($b < 100) {
		return  sprintf("%01.1f", $b) .$suffix[$i];
	} else {
		return  sprintf("%01.0f", $b) .$suffix[$i];
	}
} //end function

//class.xmail.php Version 0.4
//Requires PHP 4.1 or later
//Requires XMail 1.0 or later

class XMail {

	//////////////
	//// Vars

	var $xm_server;                        // server's name
	var $xm_ip;                                        // server's IP
	var $xm_port;                                // server's TCP port
	var $xm_ctrl_u;                        // server's CTRL user
	var $xm_ctrl_p;                        // server's CTRL password
	var $xm_ctrl_t;                        // server's CTRL type
	var $xm_err_msg;                // server's last error message
	var $xm_user_u;                        // mail user
	var $xm_user_p;                        // mail user's password
	var $xm_user_d;                        // mail user's domain
	var        $xm_ver;                                        // server version

	//////////////
	//// General functions

	// Inits object
	function init() {
		global $xm_server, $xm_ip, $xm_port, $xm_ctrl_u, $xm_ctrl_p, $xm_ctrl_t;
		$this->xm_server = $xm_server;
		$this->xm_ip = $xm_ip;
		$this->xm_port = $xm_port;
		$this->xm_ctrl_u = $xm_ctrl_u;
		$this->xm_ctrl_p = $xm_ctrl_p;
		$this->xm_ctrl_t = $xm_ctrl_t;
	} //end function

	// Executes command
	function execute($cmd, $ret_type, $vars='') {

		$fp = fsockopen($this->xm_ip, $this->xm_port, $errno, $this->xm_err_msg, 10);
		if (!$fp) {
			die($this->xm_err_msg);
		}

		$ret = explode(' ', fgets($fp, 1024)); // read server info
		$this->xm_ver = $ret[3]; // get server version

		fputs($fp, "$this->xm_ctrl_u\t$this->xm_ctrl_p\n"); // send login info
		$ret = fgets($fp, 2048); // read login info

		/*
		Upon a xmail MD5 auth failure, xmail returns a "-00171 Resource lock entry not found".
		This status doesn't fit very well and there actually is a ERR_MD5_AUTH_FAILED (-152)
		defined in the xmail errorcode table which more accurately describes what just happened.
		*/
		if (!strstr($ret,"OK")) { // not logged in
		$this->xm_err_msg = $ret; // get error msg
		return FALSE;
		echo "$ret";
		die;
		exit;
		}

		fputs($fp, "$cmd\n"); //send command
		$ret = fgets($fp, 2048); //read ret info
		//                echo "debug cmd: $cmd<br>";
		//                echo "debug ret: $ret<br>";
		//                echo "debug ret: $ret[0]<br>";
		if (!strstr($ret,"OK")) { //error exec command
		$this->xm_err_msg = $ret; // get error msg
		return FALSE;
		echo "$ret";
		die;
		exit;
		}

		switch ($ret_type) {
			case 0: //no more data expected from server
			return TRUE;
			break;
			case 1: //read data from server
			$ret = array();
			do { //read command output
			$tmp = fgetcsv($fp, 2048, "\t");
			if (count($tmp) == 1) {
				array_push($ret, $tmp[0]); //use first (only) element
			} else {
				array_push($ret, $tmp); //use all elements
			}
			} while ($tmp[0]!= '.');
			array_pop($ret); // remove trailing dot
			return($ret);
			break;
			case 2: //send vars to server
			fputs($fp, "$vars.\n"); //send command
			$ret = fgets($fp, 2048); //read ret info
			//                                return $ret;
			//                                exit;
			if ($ret[0] == '+') { //error exec command
			return TRUE;
			} else {
				$this->xm_err_msg = $ret; // get error msg
				return FALSE;
			}
			//                                      echo "$ret";
			//                                      die;
			break;
		}

	} //end function

	// Quits connection
	function quit() {
		return ('No function');
	} //end function

	// Keeps connection
	function noop() {
		$cmd = "noop";
		return ($this->execute($cmd, 0));
	} //end function

	/////////////////////
	//// Domain functions

	// List handled domains
	function domainlist() {
		$cmd = "domainlist";
		$ret = $this->execute($cmd, 1);
		sort($ret);
		return $ret;
	} //end function

	// Add a domain
	function domainadd($l_domain) {
		$cmd = "domainadd\t$l_domain";
		return ($this->execute($cmd, 0));
	} //end function

	// Delete a domain
	function domaindel($l_domain) {
		$cmd = "domaindel\t$l_domain";
		return ($this->execute($cmd, 0));
	} //end function

	// List handled domain aliases
	function aliasdomainlist() {
		$cmd = "aliasdomainlist";
		$ret = $this->execute($cmd, 1);
		sort($ret);
		return $ret;
	} //end function

	// Add a domain alias
	function aliasdomainadd($l_domain, $l_aliasdomain) {
		$cmd = "aliasdomainadd\t$l_domain\t$l_aliasdomain";
		return ($this->execute($cmd, 0));
	} //end function

	// Delete a domain alias
	function aliasdomaindel($l_domain) {
		$cmd = "aliasdomaindel\t$l_domain";
		return ($this->execute($cmd, 0));
	} //end function

	// Get custom domain file
	function custdomget($l_domain) {
		$cmd = "custdomget\t$l_domain";
		return ($this->execute($cmd, 1));
	} //end function

	// Set custom domain file
	function custdomset_org($l_domain, $l_vars) {
		$cmd = "custdomset\t$l_domain";
		$tmp = '';
		foreach($l_vars as $var) {
			if (is_array($var))
			$tmp .= implode("\t", $var) . "\n";
			else
			$tmp .= $var . "\n";
		}
		if ($tmp == "")
		$tmp = "WAIT\t0\n.\n";
		return ($this->execute($cmd, 2, $tmp));
	} //end function

	// Set custom domain file
	function custdomset($l_domain, $l_vars='') {
		$cmd = "custdomset\t$l_domain";
		if ($l_vars == "")
		$l_vars = "WAIT\t0\n.\n";
		return ($this->execute($cmd, 2, $l_vars));
	} //end function

	// List custom domains
	function custdomlist() {
		$cmd = "custdomlist";
		$ret = $this->execute($cmd, 1);
		sort($ret);
		return $ret;
	} //end function


	///////////////////
	//// User functions

	// List handled users
	function userlist($l_domain="", $l_username="") {
		$cmd = "userlist\t$l_domain\t$l_username";
		$ret = $this->execute($cmd, 1);
		sort($ret);
		return $ret;
	} //end function

	// Add user / mailinglist
	function useradd($l_domain, $l_username, $l_password, $l_usertype) {
		$cmd = "useradd\t$l_domain\t$l_username\t$l_password\t$l_usertype";
		return ($this->execute($cmd, 0));
	} //end function

	// Delete a user / mailinglist
	function userdel($l_domain, $l_username) {
		$cmd = "userdel\t$l_domain\t$l_username";
		return ($this->execute($cmd, 0));
	} //end function

	// Change a user password
	function userpasswd($l_domain, $l_username, $l_password) {
		$cmd = "userpasswd\t$l_domain\t$l_username\t$l_password";
		return ($this->execute($cmd, 0));
	} //end function

	// Authenticate a user
	function userauth($l_domain, $l_username, $l_password) {
		$cmd = "userauth\t$l_domain\t$l_username\t$l_password";
		return ($this->execute($cmd, 0));
	} //end function

	// Get user vars
	function uservars($l_domain, $l_username) {
		$cmd = "uservars\t$l_domain\t$l_username";
		$ret = $this->execute($cmd, 1);
		foreach ($ret as $item) {
			$uservars["$item[0]"] = $item[1];
		}
		return $uservars;
	} //end function

	// Set user vars
	function uservarsset($l_domain, $l_username, $l_vars) {
		$cmd = "uservarsset\t$l_domain\t$l_username\t$l_vars";
		return ($this->execute($cmd, 0));
	} //end function

	// Get user stats
	function userstats($l_domain, $l_username) {
		$cmd = "userstat\t$l_domain\t$l_username";
		$ret = $this->execute($cmd, 1);
		if ($ret) {
			foreach ($ret as $item) {
				$userstats["$item[0]"] = $item[1];
			}
		} else {
			$userstats["stats"] = 'no data available';
		}
		return $userstats;
	} //end function

	// Get mailproc.tab
	function usergetmproc($l_domain, $l_username) {
		$cmd = "usergetmproc\t$l_domain\t$l_username";
		return ($this->execute($cmd, 1));
	} //end function

	// Set mailproc.tab
	function usersetmproc($l_domain, $l_username, $l_vars) {
		$cmd = "usersetmproc\t$l_domain\t$l_username";
		return ($this->execute($cmd, 2, $l_vars));
	} //end function


	////////////////////
	//// Alias functions

	// Add user alias
	function aliasadd($l_domain, $l_alias, $l_username) {
		$cmd = "aliasadd\t$l_domain\t$l_alias\t$l_username";
		return ($this->execute($cmd, 0));
	} //end function

	// Delete a user alias
	function aliasdel($l_domain, $l_alias) {
		$cmd = "aliasdel\t$l_domain\t$l_alias";
		return ($this->execute($cmd, 0));
	} //end function

	// List handled aliases
	function aliaslist($l_domain="", $l_alias="*", $l_username="") {
		$cmd = "aliaslist\t$l_domain\t$l_alias\t$l_username";
		$ret = $this->execute($cmd, 1);
		sort($ret);
		return $ret;
	} //end function


	///////////////////////////
	//// Mailing list functions

	// Add address to mailing list
	function mluseradd($l_domain, $mlname, $mailaddress, $perms='RW') {
		$cmd = "mluseradd\t$l_domain\t$mlname\t$mailaddress\t$perms";
		return ($this->execute($cmd, 0));
	} //end function

	// Remove address from mailing list
	function mluserdel($l_domain, $mlname, $mailaddress) {
		$cmd = "mluserdel\t$l_domain\t$mlname\t$mailaddress";
		return ($this->execute($cmd, 0));
	} //end function

	// List mailing list addresses
	function mluserlist($l_domain, $mlname) {
		$cmd = "mluserlist\t$l_domain\t$mlname";
		$ret = $this->execute($cmd, 1);
		sort($ret);
		return $ret;
	} //end function


	///////////////////
	//// POP3 functions

	// Add a POP3 external link
	function poplnkadd($l_domain, $l_username, $p_domain, $p_username, $p_password, $authtype) {
		$cmd = "poplnkadd\t$l_domain\t$l_username\t$p_domain\t$p_username\t$p_password\t$authtype";
		return ($this->execute($cmd, 0));
	} //end function

	function poplnkdel($l_domain, $l_username, $p_domain, $p_username) {
		$cmd = "poplnkdel\t$l_domain\t$l_username\t$p_domain\t$p_username";
		return ($this->execute($cmd, 0));
	} //end function

	function poplnklist($l_domain='', $l_username='') {
		$cmd = "poplnklist\t$l_domain\t$l_username";
		$ret = $this->execute($cmd, 1);
		sort($ret);
		return $ret;
	} //end function

	function poplnkenable($enable, $l_domain, $l_username, $p_domain, $p_username='') {
		$cmd = "poplnkenable\t$enable\t$l_domain\t$l_username\t$p_domain\t$p_username";
		return ($this->execute($cmd, 0));
	} //end function


	/////////////////////
	//// Frozen msg functions

	// List frozen msgs
	function frozlist() {
		$cmd = "frozlist";
		return ($this->execute($cmd, 1));
	} //end function

	// Resubmit a frozen msg
	function frozsubmit($msgfile, $lev0, $lev1) {
		$cmd = "frozsubmit\t$lev0\t$lev1\t$msgfile";
		return ($this->execute($cmd, 0));
	} //end function

	// Delete a frozen msg
	function frozdel($msgfile, $lev0, $lev1) {
		$cmd = "frozdel\t$lev0\t$lev1\t$msgfile";
		return ($this->execute($cmd, 0));
	} //end function

	// Get frozen msg log file
	function frozgetlog($msgfile, $lev0, $lev1) {
		$cmd = "frozgetlog\t$lev0\t$lev1\t$msgfile";
		return ($this->execute($cmd, 1));
	} //end function

	// Get frozen msg
	function frozgetmsg($msgfile, $lev0, $lev1) {
		$cmd = "frozgetmsg\t$lev0\t$lev1\t$msgfile";
		return ($this->execute($cmd, 1));
	} //end function

	/////////////////////
	//// Config functions

	// Get a config file (relative of Mailroot)
	function cfgfileget($file) {
		$cmd = "cfgfileget\t$file";
		return ($this->execute($cmd, 1));
	} //end function

	// Save a config file (relative of Mailroot)
	function cfgfileset($file, $vars) {
		$cmd = "cfgfileset\t$file";
		$tmp = '';
		foreach($vars as $var) {
			$tmp .= implode("\t", $var) . "\n";
		}
		return ($this->execute($cmd, 2, $tmp));
	} //end function

	// Clear a config file (relative of Mailroot)
	function cfgfileclear($file) {
		$cmd = "cfgfileset\t$file";
		$tmp = " \n";
		return ($this->execute($cmd, 2, $tmp));
	} //end function

/////////////////////
//// Miscellaneous functions

	// List matching files (tested on Xmail 1.21)
	function filelist($path, $match) {
		if ($match=='') $match='*';
		$cmd = "filelist\t$path\t$match";
		$ret = $this->execute($cmd, 1);
		sort($ret);
		return $ret;
	} //end function

	// Force POP3 synchronizations (tested on Xmail 1.21)
	function startpsync(){
		$cmd[] = 'Psync \t now';
		return ($this->cfgfileset('.psync-trigger', $cmd));
	}

} // end class

/////////////////////
//// WebGUI functions (has nothing to do with XMail)

// List controlled servers
function serverlist() {
	global $cfg_servers;
	$servers = array();
	if (file_exists($cfg_servers)) {
		do_chmod($cfg_servers); // check file
		$fp = fopen($cfg_servers, 'r'); //open server info
		while ($line = @fgetcsv($fp, 1024, "\t")) {
			$servers["$line[0]"] = array_slice($line, 1);
		}
		fclose($fp);
	}
	ksort($servers);
	return $servers;
} //end function

// Writes controlled servers
function serverwrite($servers) {
	global $cfg_servers;
	if (!file_exists($cfg_servers)) { //file check
	touch($cfg_servers); //create empty file
	}
	do_chmod($cfg_servers); // check file
	$fp = fopen($cfg_servers, 'wb');
	foreach($servers as $svr_name => $svr_info) {
		$line = $svr_name . "\t" . implode("\t", $svr_info) . "\n";
		fwrite($fp, $line);
	}
	fclose ($fp);
	return true;
} //end function

// Test SSH link
function test_ssh($user, $server) {
	if (strpos(`cat ~/.ssh/known_hosts2`, $server) === false)
	return false;
	if (`ssh $user@$server echo -n ok` == 'ok')
	return true;
	return false;
}


//writes configuration
function write_conf($file,$find,$replace) {
	$fh = fopen($file, "r");
	if (!$fh) {
		echo "Could not open $file";
	} else {
		$content = "";
		while (!feof($fh)) {
			$line = fgets($fh, 4096);
			$content .= ereg_replace($find ,$replace, $line);
		}
		fclose($fh);
		$fh = fopen($file, "w");
		fwrite($fh, $content);
		fclose($fh);
	}
}

//chmod file
function do_chmod($fname){
	$perms = substr(base_convert(fileperms($fname), 10, 8), 3);
	if ($perms != "666"){
		$chmod = chmod ("$fname", 0666);
		if (!$chmod){
			echo "<b>Your server does not allow automatic file chmoding</b><br>";
			echo "<b>You have to manually chmod 666 $fname/make it writable</b><br>";
			die;
		}

	}
}

//limit chars
function limitch($value,$lenght){
	if (strlen($value) >= $lenght ){
		$limited = substr($value,0,$lenght);
		$limited .= "...";
	}
	return $limited;
}
//limit chars reversed
function limitchrev($value,$lenght){
	if (strlen($value) >= $lenght ){
		$start = strlen($value)- $lenght;
		$limited = "...";
		$limited .= substr($value,$start,$lenght);
	}
	return $limited;
}

?>