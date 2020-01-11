<?php
require_once('class.xmail.php');
require_once('config.php');
require_once('environment.inc');
session_start();

// in the future releases the following statement may be
// transferred in the 'CONFIG.PHP' and then managed
// by the page 'CONFIG.INC'
$cfg_allow_caption_info='on';

if ($cfg_debug_stat == "on") {
	error_reporting(E_ALL);
}

// import some vars
if (isset($_SESSION['mail_server']))
$mail_server = $_SESSION['mail_server'];
else      $mail_server = new XMail();

if (isset($_REQUEST['action'])){
	$action = $_REQUEST['action'];
} else {
	if (count(serverlist($cfg_servers))){
		$action = 'login';
	} else {
		$action = 'serverlist';
	}
}

if (isset($_REQUEST['domain']))
$domain = $_REQUEST['domain'];
else     $domain = '';

if (isset($_REQUEST['username']))
$username = $_REQUEST['username'];
else     $username = '';

// set defaults to prevent url spoofing
switch ($mail_server->xm_ctrl_t) {
	case 'r_user':
		$domain = $mail_server->xm_user_d;
		$user = $mail_server->xm_user_u;
		$type = 'r_user';
		if (empty($username)) $username = $user;
	case 'd_admin':
		$domain = $mail_server->xm_user_d;
		$type = 'd_admin';
	case 's_admin':
		$type = 's_admin';
}

//if (empty($_POST) && $action =="login") {
if ($action =="login") {
	// Set session cookie: later it will be
	// checked if session cookie has been blocked or not
	// (Be sure that on the server side in php.ini the "session.use_cookies" is set to true)
	if (empty($_GET['reloaded'])) {
		$_SESSION['InfoAreReadable'] = true;
		// Reload the page, so session values will be available
		header ("Location: main.php?action=login&reloaded=yes");
	}
}

// get/set some vars (usefull for the dynamic part of menu titles...)
if (isset($_REQUEST['ftype'])) $filtertype = $_REQUEST['ftype']; else $filtertype = '';
if (isset($_REQUEST['svr'])) $svr = $_REQUEST['svr']; else $svr = '';
if (isset($_REQUEST['alias'])) $alias = $_REQUEST['alias']; else $alias = '';
if (isset($_REQUEST['mlname'])) $mlname = $_REQUEST['mlname']; else $mlname = '';
if (isset($_REQUEST['log'])) {
	$ltype = $_REQUEST['ltype'];
	$log = $_REQUEST['log'];
} else {
	$ltype = '';
	$log = 'Empty Log';
}
if (isset($_GET['ltype'])){
	switch ($_GET['ltype']) {
		case 'ctrl': $logtype = 'Administration Server log records';	break;
		case 'fing': $logtype = 'Finger Server log records';	break;
		case 'filters': $logtype = 'SMTP Filter log records';	break;
		case 'lmail': $logtype = 'Local Mail Server log records';	break;
		case 'pop3': $logtype = 'Pop3 Server log records';	break;
		case 'psync': $logtype = 'Pop3 Synch Server log records';	break;
		case 'smail': $logtype = 'SendMail Server log records';	break;
		case 'smtp': $logtype = 'SMTP Server log records';	break;
		default: break;
	}
}

?>
<html>
<head>
        <link rel="stylesheet" href="main.css" type="text/css">
</head>
<body>
<img src='gfx/mail.gif' height='99' width='450' border='0'>
<div class='content'>

<?php


//#####################################################################################################
require_once('class.phpxm.php');
$mnu = new MenuTree();

// DEFINE USER TYPES AND THEIRS BINARY AUTH WEIGHTS (1,2,4,8,16,..,2^n)
$mnu->AddUserType('anonymous', 1);
$mnu->AddUserType('r_user', 2);
$mnu->AddUserType('d_admin', 4);
$mnu->AddUserType('s_admin', 8);

// DEFINE MENU STRUCTURE =============================================================================================================
// (best view with 'tab' of 4 characters)
// AUTH is in binary form and it represents the sum of the AUTH weigths (may be used also a decimal value)
// examples: '1000' => s_admin; '1100' => s_admin + d_admin; '0010' => r_user
// note: '1000' = 8; '0100' = 4; '1100' = 12 (8+4); '0010' = 2
//
//  ->Add('MenuAction'  'FileToInclude.inc'		 'AUTH'	 'ParentMenu'   #  'Title \t subtitle'  'parameters'  'Info tips/caption)';

// 000 - login files
$mnu->Add('login',		'login.inc', 			 '1111', '', 	   		0, 'Login', '', '');		// login to server
$mnu->Add('logout',		'logout.inc', 			 '1111', '', 	   		0, 'Logout', '', '');	// logout from server

// 100 - domain files
$mnu->Add('domainlist',	'domain/domainlist.inc', '1000', '', 	   		0, 'server domains|domains', '', 'List of all the served domains');	// list domains
$mnu->Add('domainadd',	'domain/domainadd.inc',  '1000', 'domainlist', 	0, 'add new domain', '', '');	// create new domain
$mnu->Add('domaindel',	'domain/domaindel.inc',  '1000', 'domainlist', 	0, 'remove "$domain"', '', '');	// delete domain
$mnu->Add('custdomdel',	'domain/domaindel.inc',  '1000', 'domainlist', 	0, 'remove custom domain', '', '');	// delete custom domain
$mnu->Add('custdomget',	'domain/custdomget.inc', '1000', 'domainlist', 	0, '$domain (custom domain)', '', '');	// get custom domain
$mnu->Add('custdomset',	'domain/custdomset.inc', '1000', 'domainlist', 	0, 'add new custom domain', '', '');	// set custom domain

// 200 - user files
$mnu->Add('userlist',	'user/userlist.inc', 	 '1100', 'domainlist', 	1, '$domain \t mailboxes', '&domain=$domain', 'List of all the mailboxes of this domain');	// list users
$mnu->Add('vxnode202',	'', 	 				 '1100', 'domainlist', 	2, '$domain \t mailing lists', '', '');	// start submenu mailing lists
$mnu->Add('domuserdef',	'usrdefaults.inc', 		 '1100', 'domainlist', 	3, '$domain \t users defaults', '&domain=$domain', 'Variables and values that must be used by default when creating new users in $domain');	// set default values for users
$mnu->Add('domainmproc','domain/domainmproc.inc','1100', 'domainlist', 	4, '$domain \t domain mail proc', '&domain=$domain', 'Mail processes that are common for all users in this domain');	// set default values for users
$mnu->Add('domsettings','domain/domsettings.inc','1000', 'domainlist', 	9, '$domain \t domain settings', '&domain=$domain', 'Special settings for the domain <b>"$domain"</b>');	// set default values for users
$mnu->Add('useradd',	'user/useradd.inc', 	 '1100', 'userlist',   	0, 'add new user', '&domain=$domain', '', '');	// add user / maillist
$mnu->Add('userdel',	'user/userdel.inc', 	 '1100', 'userlist',   	0, 'remove user', '&domain=$domain&username=$username', '', '');	// delete user / maillist

// 201 - user files MAILBOXES
$mnu->Add('uservars',	'user/uservars.inc', 	 '1100', 'userlist',   	1, '$username@$domain \t account', '&domain=$domain&username=$username', 'Account settings for <b>"$username@$domain"</b>');	// user vars
$mnu->Add('userchange',	'user/userchange.inc',   '0010', 'userlist',    1, '$username@$domain \t settings', '&domain=$domain&username=$username', 'Change settings for "$username@$domain"');	 // user allowed changes
$mnu->Add('aliaslist',	'user/aliaslist.inc', 	 '1110', 'userlist',   	2, '$username@$domain \t aliases',  '&domain=$domain&username=$username', 'Virtual names (aliases) settings for <b>"$username@$domain"</b>');	// list aliases
$mnu->Add('poplnklist',	'user/poplnklist.inc',   '1100', 'userlist',   	3, '$username@$domain \t ext POP3', '&domain=$domain&username=$username', 'Preset an external mailboxes from which to collect messages');	// list pop3s
$mnu->Add('pop3ipmap',	'user/pop3ipmap.inc',    '1100', 'userlist',   	4, '$username@$domain \t pop clients', '&domain=$domain&username=$username', 'IP settings from which a POP3 client may access to this mailbox');	// user vars
$mnu->Add('usergetmproc','user/usergetmproc.inc','1100', 'userlist',    5, '$username@$domain \t user mail proc', '&domain=$domain&username=$username', 'Mail processes settings for <b>"$username@$domain"</b> mailbox');	// user IP (POP3) permissions
$mnu->Add('aliasadd',	'user/aliasadd.inc', 	 '1110', 'aliaslist',  	0, 'add new alias',  '&domain=$domain&username=$username', '');	// add alias
$mnu->Add('aliasdel',	'user/aliasdel.inc', 	 '1110', 'aliaslist',  	0, 'remove alias $alias',  '&domain=$domain&username=$username&alias=$alias', '');	// remove alias
$mnu->Add('poplnkadd',	'user/poplnkadd.inc',    '1110', 'poplnklist', 	0, 'add new POP3', '&domain=$domain&username=$username', '', '');	// add pop3
$mnu->Add('poplnkdtl',	'user/poplnkdtl.inc',    '1110', 'poplnklist', 	0, '$p_username@$p_domain', '', '');	// pop3 dtl
$mnu->Add('poplnkdel',	'user/poplnkdel.inc',    '1110', 'poplnklist', 	0, 'remove POP3 link', '', ''); // del pop3 link

// 202 - user files MAILING LIST
$mnu->Add('maillists',	'user/maillists.inc', 	 '1100', 'vxnode202', 	1, 'mailing lists', '&domain=$domain', 'List of all the mailing lists of this domain');	// list mailing lists
$mnu->Add('mluservars',	'user/mluservars.inc',	 '1100', 'maillists', 	1, '$mlname \t account', '&domain=$domain&mlname=$mlname', 'Account for the mailing list');	// list mailing lists
$mnu->Add('mluserlist',	'user/mluserlist.inc',   '1100', 'maillists',   2, '$mlname \t addresses', '&domain=$domain&mlname=$mlname', 'List of mailing addresses'); // list mailing list users
$mnu->Add('mailtolist',	'sendmail.inc', 		 '1100', 'maillists', 	3, '$mlname \t send mail to list', '&domain=$domain&mlname=$mlname', 'Send a message to the whole mailing list'); // send mail
$mnu->Add('mllistdel',	'user/mllistdel.inc',    '1100', 'maillists', 	0, 'remove', '&domain=$domain&username=$username', ''); // del mailing list
$mnu->Add('mllistadd',	'user/useradd.inc', 	 '1100', 'maillists',  	0, 'add new list', '&domain=$domain', '', '');	// add maillist
$mnu->Add('mluseradd',	'user/mluseradd.inc',    '1100', 'mluserlist', 	0, 'add address', '', 'Add a new user address to the mailing list'); // add mailing list users
$mnu->Add('mluseredit',	'user/mluseredit.inc',   '1100', 'mluserlist', 	0, 'edit address', '', 'Edit user rights for this address'); // edit mailing list users
$mnu->Add('mluserdel',	'user/mluserdel.inc',    '1100', 'mluserlist', 	0, 'remove address', '', ''); // del mailing list users

// 300 - send mail
$mnu->Add('sendmail',	'sendmail.inc', 		 '1100', 'mluserlist', 	0, 'send message', '', ''); // send mail

// 400 - servers setup
$mnu->Add('serverlist',	'server/serverlist.inc', '1001', '', 	   		0, 'phpxmail setup|setup', '', 'List of Xmail servers to which PHPXmail it can be connected');	// list servers
$mnu->Add('serveradd',	'server/serveradd.inc',  '1001', 'serverlist', 	0, 'add server', '', 'Add an Xmail server to the PHPXmail connection list');	// add server
$mnu->Add('serverdel',	'server/serverdel.inc',  '1000', 'serverlist', 	0, 'remove server', '', 'Remove an Xmail server from the PHPXmail connection list');	// remove server
$mnu->Add('servercfg',	'server/servercfg.inc',  '1000', 'serverlist', 	0, '$svr', '', '');	// configure server

// 401 - server configs
$mnu->Add('vxnode401',	'', 					 '1000', '', 	   		0, '', '', '');	// push submenu down of 1 level (disconnect from root level)
$mnu->Add('servervars',	'server/servervars.inc', '1000', 'vxnode401',  	1, 'server config \t config', '', 'Xmail server configuration variables');	// list server config
$mnu->Add('usrdefaults','usrdefaults.inc', 		 '1000', 'vxnode401',  	4, 'server config \t users defaults', '', 'Global default variables and values that must be used when creating a new user<br>(They are common for all the served domains, unless specific domain settings)');	// set default values for users
$mnu->Add('vxnode402',	'', 	 				 '1000', 'vxnode401',  	3, 'server config \t IP maps', '', '');	// group IP-MAPs in a sub menu
$mnu->Add('servercmd',	'server/servercmd.inc',	 '1000', 'vxnode401',  	5, 'server config \t commands', '', 'Miscellaneous commands for the Xmail server');	// send commands to xmail server

// 402 - server IP Access Settings
$mnu->Add('srvipmpop3',	'server/ipmap.inc', 	 '1000', 'vxnode402',  	2, 'IP maps \t POP3', '&iptype=pop3', 'POP3 server global access settings');	// Global POP3 access
$mnu->Add('srvipmsmtp',	'server/ipmap.inc', 	 '1000', 'vxnode402',  	1, 'IP maps \t SMTP', '&iptype=smtp', 'SMTP server global access settings');	// Global SMTP access
$mnu->Add('srvipmfinger','server/ipmap.inc', 	 '1000', 'vxnode402',  	3, 'IP maps \t Finger', '&iptype=finger', 'FINGER server access settings'); //Global FINGER access
$mnu->Add('srvipmctrl', 'server/ipmap.inc', 	 '1000', 'vxnode402',  	4, 'IP maps \t Control', '&iptype=ctrl', 'Xmail remote control access settings'); //server CONTROL access

// 500 - server filters
$mnu->Add('vxnode501',	'', 					 '1000', '', 	   		0, '', '', '');	//  push submenu down of 1 level (disconnect from root level)
$mnu->Add('serverfilt',	'server/serverfilt.inc', '1000', 'vxnode501', 	1, 'server filters \t filters', '', 'Filters panel control');	// server filters list

$mnu->Add('serverfilters','server/serverfilters.inc','1000','serverfilt',0, '$filtertype', '', '');	// server filters
$mnu->Add('filterlist', 'server/filterlist.inc', '1000', 'vxnode501',   2, 'server filters \t files list', '', 'List and usage of all filter files available on the server');
$mnu->Add('filterdel',	'server/filterdel.inc',	 '1000', 'filterlist',  0, 'remove filter',  '', '');	// Remove a filter file
$mnu->Add('filteradd',	'server/filteradd.inc',	 '1000', 'filterlist',  0, 'add new file',  '', 'Create a new empty filter file on server');	// Add a filter file
$mnu->Add('filteredit',	'server/filteredit.inc', '1000', 'filterlist',  0, 'edit file',  '', 'Modify a filter file on server');	// Add a filter file

// 550 - server spam list
$mnu->Add('vxnode551',	'', 					 '1000', '', 	   		0, '', '', '');	//  push submenu down of 1 level (disconnect from root level)
$mnu->Add('spamlist',   'server/spamlist.inc',   '1000', 'vxnode551',   1, 'server local spam list|server spam \t e-mail', '', 'All the emails coming from the listed addresses will be blocked by the server');
$mnu->Add('spamiplist', 'server/spamiplist.inc', '1000', 'vxnode551',   2, 'server local spam list|server spam \t IP address', '', 'Action performed by the server for all the emails coming from each listed IP address');
$mnu->Add('spamadd',	'server/spamadd.inc', 	 '1000', 'spamlist',  	0, 'add new spammer address',  '', 'Add an e-mail address to the local spam address list');	// add spam address
$mnu->Add('spamipadd',	'server/spamipadd.inc',	 '1000', 'spamiplist', 	0, 'add new spammer IP',  '', 'Add an IP address to the local spam address list');	// add spam IP address
$mnu->Add('spamipedit',	'server/spamipadd.inc',	 '1000', 'spamiplist', 	0, 'edit spammer IP',  '', 'Edit an IP address in the local spam address list');	// add spam IP address

// 600 - server logs
$mnu->Add('serverlogs',	'server/serverlogs.inc', '1000', '', 	   		0, 'server logs', '','', '');	// server logs
$mnu->Add('serverlog',	'server/serverlog.inc',  '1000', 'serverlogs', 	0, '$ltype $log', '', '$logtype');	// server log viewing / delete

// 700 - froz messages
$mnu->Add('frozlist',	'server/frozlist.inc',   '1000', '', 	   		0, 'server spool|frozen messages', '', ''); // list frozen msgs
$mnu->Add('frozdel',	'server/frozdel.inc',    '1000', 'frozlist',   	0, 'delete', '', ''); // delete frozen msgs
$mnu->Add('frozsubmit',	'server/frozsubmit.inc', '1000', 'frozlist',   	0, 'resubmit', '', ''); // resubmit frozen msgs
$mnu->Add('frozgetlog',	'server/frozgetlog.inc', '1000', 'frozlist',   	0, 'get log', '', ''); // get frozen msg log
$mnu->Add('frozgetmsg',	'server/frozgetmsg.inc', '1000', 'frozlist',   	0, 'view message', '', ''); // get frozen msg

// 800 - others
$mnu->Add('update',		'update.inc', 			 '1000', '',     		0, 'check for new version', '', ''); // check new version
$mnu->Add('config',		'config.inc', 			 '1000', '',     		0, 'phpxmail config', '', 'PHPXmail configuration options'); // edit configuration
$mnu->Add('legend',		'legend.inc', 			 '1111', '',     		0, 'phpxmail icons legend', '', 'List of all the icons used in this application'); // show icon legend
$mnu->Add('register',	'register.inc', 		 '1111', '',     		0, 'Register', '', 'Create your own new mailbox'); // visitor register

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// Check if the access is from a regular user, then modify menues accordling
if (!empty($mail_server->xm_ctrl_t)){
	if ($mail_server->xm_ctrl_t == 'r_user'){
		$all = $mail_server->userlist($domain, $username);
		// split in users and lists
		$lists = array();
		foreach ($all as $id => $mlcheck) {
			if (strtoupper($mlcheck[3]) == 'M') {
				$mlname = $username;
				$mnu->Add('mlusrchglst',	'user/mlusrchglst.inc',   '0010', 'userlist',    	3, '$username@$domain \t addresses', '&domain=$domain&mlname=$mlname', 'List of mailing addresses'); // list mailing list users
				$mnu->Add('mlusrchgadd',	'user/mlusrchgadd.inc',   '0010', 'mlusrchglst', 	0, 'add address', '', 'Add a new user address to the mailing list'); // add mailing list users
				$mnu->Add('mlusrchgedit',	'user/mlusrchgedit.inc',  '0010', 'mlusrchglst', 	0, 'edit address', '', 'Edit user rights for this address'); // edit mailing list users
				$mnu->Add('mlusrchgdel',	'user/mlusrchgdel.inc',   '0010', 'mlusrchglst', 	0, 'remove address', '', ''); // del mailing list users
				// remove the Alias menu item
				$mnu->Nodes['aliaslist']['access'] = "1100";
			}
		}
	}
}
// enable/disable the caption (tip) at the top of each page
$mnu->ShowCaption = ($cfg_allow_caption_info == 'on');

// preset the default page to show when access will be denied
$mnu->DeniedPage = 'denied.inc';

// preset special messages (in this case info about the Cookies needs...)
if (empty($_SESSION['InfoAreReadable']) && $action == 'login') {
	// authorized session cannot be initiated and/or mantained, discover why:
	if (ini_get("session.use_cookies")) {
		// session cookies are not enabled on client side
		// in certain systems it happen also if 3rd party cookies are not enabled
		$mnu->SpecialNotes = "</p><img src=gfx/ico_warn.gif> Login will be possible only if your system allows the use of cookies";
	} else {
		// session cookies are not enabled on server side
		// depending by local settings and use, it seems not to be a problem on certain systems when used locally;
		// anyway it's better to report it, so it will not be surprises when used in remote mode
		$mnu->SpecialNotes = "</p><table><tr><td><img src=gfx/ico_warn.gif></td><td> Warning: since in PHP.INI on the server 'session.use_cookies' is not set to true<br> it could prevents the login on certains systems</td></tr></table>";
	}
}

// if the requested action is authorized for this user, then include the relative file
if ($mnu->isValidAction($action, $mail_server->xm_ctrl_t)) {
	include($mnu->FileToInclude);
}

?>
</div>
</body>
</html>