<?PHP
//=====================================================================================================================
//
//	Code developed initially for PHPXmail (Web Interface for Xmail server)
//
//	Licensing:	you may freely redistribute and modify this code (provided that this header are not removed from here
//				and that you e-mail me at below_zero@sourceforge.net about its use in an application, or about changes
//  			and/or improvements: thank you!)
//
//=====================================================================================================================
//
//	Curr. Rev.: 1.1
//
//	History Log: - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//
//	Rev. 1.1
//  Date:		Aug, 29th 2005
//	Author:		Marco Riccardi
//	e-mail:		below_zero@sourceforge.net
//  Changes:	Added section 'MISCELLANEOUS FUNCTIONS', fixed typo errors and file header
//
//	Rev. 1.0	(1st release)
//  Date:		April, 11th 2005
//	Author:		Marco Riccardi
//	e-mail:		below_zero@sourceforge.net
//
//=====================================================================================================================
//
//	C L A S S  'M e n u T r e e'
//
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

class MenuTree {

	var $ShowCaption;	//INPUT: boolean to enable the caption descritpion
	var $DeniedPage;	//INPUT: file name of the page that must be included on deny access (before to die)
	var $SpecialNotes;	//INPUT: text (or HTML code) that must be echoed BEFORE open the file spicified by 'Action'
	var $UserTypes;		//INPUT: contain user types & theirs relative auth. weights
	var $FileToInclude;	//OUTPUT: contain the file name to include
	var $Nodes;			//OUTPUT: contain the whole menu tree structure

	function MenuTree (){
		// defaults is for show info tips
		$this->ShowCaption = true;
		$this->SpecialNotes = "";
		// initialize tree menu inserting root's level.
		$this->Nodes = array();
		$this->Nodes[''] = array('node'=>'', 'filename'=>'', 'access'=>0, 'parent'=>'',
		'ordinal'=>0, 'title'=>'', 'linkparms'=>'', 'caption'=>'');
	}

	// create user types table, $BinaryWeight must be: 1, 2, 4, 8, 16
	// examples:
	//
	// myMenuClass->AddUserType('anonymous', 1)
	// myMenuClass->AddUserType('r_user', 2)
	// myMenuClass->AddUserType('d_admin', 4)
	//
	// (remember that the user with less authorization must have weigth = 1)
	//
	function AddUserType ($UserType, $BinaryWeight){
		if (!isset($this->UserTypes)) $this->UserTypes = array();
		$this->UserTypes["$BinaryWeight"] = $UserType;
	}

	// PUSH A MENU NODE IN THE MENU TREE. Examples:
	//
	// myMenuClass->Add('login',  'login.inc',  '1111', '', 0, 'Login', '', 'login to server');
	// myMenuClass->Add('logout', 'logout.inc', '1110', '', 0, 'Logout', '', 'logout from server');
	//
	// remember that virtual nodes must have a name starting with the 'vxnode' reserved keyword
	//
	// Virtual nodes may not have files to include and they are usefull when grouping submenus
	// (Virtual nodes must be used when the group parent of submenues is at root level).
	//
	// IMPORTANT: Submenues must have ordinal value > 0 and the \t separator in title field

	function Add($NodeName, $FileName, $Priv, $ParentNode, $Ordinal, $Title, $linkparms, $Caption ){
		$this->Nodes[$NodeName] = array('node'=>$NodeName, 'filename'=>$FileName, 'access'=>$Priv, 'parent'=>$ParentNode,
		'ordinal'=>$Ordinal, 'title'=>$Title, 'linkparms'=>$linkparms, 'caption'=>$Caption);
	}

	function isValidAction($action, $CurrentUser){
		// check if permission table was preset
		if (!isset($this->UserTypes) ){
			include($this->DeniedPage);
			die;
		}
		if (!isset($CurrentUser)) {
			// if is a 'non logged' user, assign to him the lowest privileges
			// so him may access at least to LOG-IN page
			$this->CurrentUser = $this->UserTypes['1'];
		} else {
			$this->CurrentUser = $CurrentUser;
		}
		if ($this->is_MenuNode($action)) {
			// show navigation header
			echo $this->BuildPageHeader ($action, $CurrentUser);
			// check if call is permitted
			$this->check_privileges($this->Nodes[$action]['access'], true);
			$this->FileToInclude = $this->Nodes[$action]['filename'];
			if ($this->is_ExistingFile($this->Nodes[$action]['filename'])) {
				return true;
			} else {
				echo '<table><tr valign="top">';
				echo '<td><img src=gfx/ico_stop.gif align=absmiddle title=info>&nbsp;</td>';
				echo "<td>Sorry: <b>'$action'</b> not supported yet.<br>(File <b>'" .$this->FileToInclude ."'</b> was not found)";
				echo '</tr></table>';
				echo '<hr></p><a href="javascript:history.go(-1)">Back</a></td>';
				die;
			}
		} else {
			echo "<img src=gfx/ico_stop.gif align=absmiddle title=info>&nbsp;Sorry: <b>'$action'</b> not supported yet";
		}
		return false;
	}

	// ONLY THE ABOVE FUNCTIONS MUST BE USED AFTER INSTANCING OF THIS CLASS ###########################################
	// build navigation menu...
	function BuildPageHeader ($action, $CurrentUser){
		if (isset($CurrentUser)) $this->CurrentUser = $CurrentUser; else $this->CurrentUser = $this->UserTypes['1'];
		// sort the submenues, making easy to build a navigation header...
		foreach ($this->Nodes as $key => $row) {
			$rowParent[$key]  = $row['parent'];
			$rowOrder[$key] = $row['ordinal'];
			$rowAction[$key] = $row['node'];
		}
		array_multisort($rowParent, SORT_ASC, $rowOrder, SORT_ASC, $rowAction, SORT_ASC, $this->Nodes); // $data as the last parameter, to sort by the common key
		$NavHeader_1 = array();
		$recurr = $action;
		$i = false;
		// build the 1st nav-menu line (like:  Domains >> mydomain.com >> mymailbox )
		while ($this->Nodes[$recurr]['parent'] != $this->Nodes[$recurr]['node']) {
			// do this loop until it reach the root of menu tree
			$LongTitle = $this->getfromtitle($this->Nodes[$recurr]['title'], 'long');
			$ShortTitle = $this->getfromtitle($this->Nodes[$recurr]['title'], 'short');
			// if there something for the 1st row and if user have access then push it in nav array
			if ($LongTitle != '' && $this->check_privileges($this->Nodes[$recurr]['access'], false, '')) {
				// (do not make a clickable link for the current page nor for a non valid access)
				if ($i) {
					if (strstr($this->Nodes[$recurr]['node'], 'vxnode') == false){
						$temp = "<a href='main.php?action=" .$this->Nodes[$recurr]['node'] .str_replace_vars($this->Nodes[$recurr]['linkparms']) ."' target='main'>";
						$temp = $temp .str_replace_vars($ShortTitle) ."</a>";
					} else {
						//virtual nodes musk be skipped: go one level top
						$temp = $this->VxNodeParent($this->Nodes[$recurr]['node'], $ShortTitle);
					}
					array_unshift ( $NavHeader_1, $temp);
				} else {
					$i=true;
					$parent = $this->Nodes[$recurr]['parent'];
					// use the 1st section of title
					array_unshift ( $NavHeader_1, str_replace_vars($LongTitle));
				}
			}
			$recurr = $this->Nodes[$recurr]['parent'];
		}
		$retstring = '<h3>' .implode(" >> ", $NavHeader_1) .'</h3>';

		// now check for the 2nd nav-menu line  (like:  sub1 || sub2 || sub3 || ... || subn )
		if ($this->Nodes[$action]['ordinal'] != 0) {
			// this menu have other brothers at the same level
			$Brothers = array();		//will contain all the submenues
			$BrothersList = array();		//will contain the HML tags for the admitted sub-menues
			foreach ($this->Nodes as $sons){
				if ($sons['parent'] == $parent && $sons['ordinal'] != 0){
					array_push ($Brothers, $sons);
				}
			}
			// now build the 2nd nav-menu line
			foreach ($Brothers as $brother){
				//show only if the user have access to the link
				if ($this->check_privileges($brother['access'], false, '')){
					//get the 2nd part of (sub title)
					$SubTitle = $this->getfromtitle($brother['title'], 'sub');
					if ($brother['node'] == $action && trim($brother['filename']) != '') {
						//do not make link for the current page
						array_push($BrothersList, '<span class="caption">' .str_replace_vars($SubTitle) .'</span>');
					} else {
						if (strstr($brother['node'], 'vxnode') == false){
							$temp = "<a href='main.php?action=" .$brother['node'];
							$temp .= str_replace_vars($brother['linkparms']) ."' target='main'>";
							$temp = $temp .str_replace_vars($SubTitle) ."</a>";
						} else {
							$temp = $this->FirstVxNodeSon($brother['node'], $SubTitle);
						}
						array_push ($BrothersList, $temp);
					}
				}
			}
			unset($Brothers);
			$retstring .= implode(" || ", $BrothersList) .'</p>';
		}
		// if it's present an Info Caption (and it's enabled) then show it
		if ($this->Nodes[$action]['caption'] != '' && $this->ShowCaption) {
			$retstring .= '<table><tr valign="top">';
			$retstring .= '<td><img src=gfx/ico_info.gif align=absmiddle title=info>&nbsp;</td>';
			$retstring .= '<td>' .str_replace_vars($this->Nodes[$action]['caption']) .'</td>';
			$retstring .= '</tr></table>';
		}
		return $retstring .$this->SpecialNotes .'<hr></p>';
	}

	function check_privileges($code, $mustDie) {
		$allowed = array();
		if (is_string($code)) $dec_code = bindec($code); else $dec_code = $code;
		$i=1;
		for ($k=0; $k < count($this->UserTypes); $k++){
			if ($dec_code & $i) $allowed[] = $this->UserTypes["$i"];
			$i=$i*2;
		}
		if (!in_array($this->CurrentUser, $allowed)) {
			if ($mustDie) {
				include($this->DeniedPage);
				die;
			} else return false;
		}
		return true;
	}

	// check if this menu node exist...
	function is_MenuNode($needle){
		foreach($this->Nodes as $value) {
			if($value['node']==$needle) return true;
		}
		return false;
	}

	// check if this menu node exist...
	function is_ExistingFile($filename){
		if (!isset($filename)) return true;
		if (trim($filename) == '') return true;
		return is_file($filename);
	}

	// get the selected part of menu title
	function getfromtitle($title, $selection){
		$temp2 = explode ('\t', $title);
		$temp1 = explode ('|', $temp2[0]);
		if (!isset($temp1[1])) $temp1[1] = $temp1[0];
		switch ($selection) {
			case 'long':
				return trim($temp1[0]);
				break;
			case 'short':
				return trim($temp1[1]);
				break;
			case 'sub':
				return (empty($temp2[1])) ? "" : trim($temp2[1]);
				break;
		}
		// wrong request:
		return $selection;
	}
	// build a link for the parent of virtual node; if also
	// the parent is a virtual node then get the first son of parent
	function VxNodeParent($VxNode, $text){
		$parent = $this->Nodes[$VxNode]['parent'];
		//		if (strstr($parent, 'vxnode') == false){
		//			// check for privileges
		//			if ($this->check_privileges($this->Nodes[$VxNode]['access'], false, '')) {
		//				$temp = "<a href='main.php?action=" .$this->Nodes[$parent]['node'] .str_replace_vars($this->Nodes[$parent]['linkparms']) ."' target='main'>";
		// 				$temp .= str_replace_vars($this->getfromtitle($this->Nodes[$parent]['title'], 'short')) ."</a>";
		//			}
		//		} else {
		$temp = $this->FirstVxNodeSon($parent, $text);
		//		}
		return $temp;
	}

	// build a link for the first son of virtual node
	function FirstVxNodeSon($parent, $text){
		foreach ($this->Nodes as $sons){
			if ($sons['parent'] == $parent && $sons['ordinal'] != 0 ){
				// check for privileges
				if ($this->check_privileges($this->Nodes[$sons['node']]['access'], false, '')) {
					$temp  = "<a href='main.php?action=" .$sons['node'];
					$temp .= str_replace_vars($sons['linkparms']) ."' target='main'>";
					if (!isset($text)) {
						$temp .= str_replace_vars($this->getfromtitle($sons['title'], 'short')) ."</a>";
					} else {
						$temp .= str_replace_vars($text) ."</a>";
					}
				} else {
					$temp = '';
				}
				return $temp;
			}
		}
		// this link does not have any submenues
		return $text .' (broken link)';
	}
}

//replace in string a variable with its value
function str_replace_vars($string){
	$pattern = "[ &=\"@\t]";
	$temp = $string;
	$words = preg_split($pattern, $temp);
	foreach ($words as $word) {
		if (strchr($word, "$") == $word) {
			global ${str_replace("\$", "", $word)};
			$temp = str_replace ($word, ${str_replace("\$", "", $word)}, $temp);
		}
	}
	return $temp;
}

//=====================================================================================================================
//
//	C L A S S  'I P _ o b j e c t'
//
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//	This class operate on IPs, it checks the formal validity and converts ranges and masks.
//	After invoked, the class expose two functions:
//	the main function 'trueIP' and the auxiliary 'BitsToIPMask'
//
//	1)	function 'trueIP': it is possible to submit:
//		a) IP (like '192.168.100.0') or
//		b) IP range (like '192.168.100.0/20') or
//		c) MASK (like '255.240.0.0')
//
//		it return TRUE / FALSE
//		if it return true, then flags and conversions are done conseguently:
//
//  	a good IP sets the following properties
//			1 - $ValidIP to true,
//
//		a good IP RANGE sets the following properties:
//			1 - $ValidIP to true,
//			2 - $ValidRange to true,
//			3 - $Mask at the converted value of range: if RANGE=24 then MASK='255.255.255.0'
//
//		a good MASK sets  the following properties:
//			1 - $ValidRange to true,
//			2 - $Range at the converted value of mask: if MASK='255.255.240.0' then $Range=20 (bits)
//
//	2)	function 'BitsToIPMask': it is possible to submit a number between 0 and 32.
//		if it return tue, then the property $Mask will contain the converted value of mask bits
//
class  IP_object {
	var $IP;
	var $Mask;
	var $Range;
	var $ValidIP;
	var $ValidMask;
	var $ValidRange;

	function IP_object(){
		$this->IP = '';
		$this->Mask = '';
		$this->Range = '';
		$this->ValidIP = false;
		$this->ValidMask = false;
		$this->ValidRange = false;			// active if '192.168.0.0/24'
	}
	//accept IP (for IP and mask) or IP/xx like: 192.168.100.4 or 192.168.100.4/24 or 255.255.255.0
	//and return all the components
	function trueIP($IP, $isMask) {
		if (!isset($isMask)) $this->isMask = false; else $this->isMask = $isMask;
		if (strspn(trim($IP), '0123456789./') != strlen(trim($IP))) return false;
		$this->s_arr = array();// obtain the four IP numbers...
		$this->s_arr = explode('.', trim($IP));

		$IP_flags = 0; //ponderal weigth
		if ($this->isMask) {
			//threat as mask
			$this->ValidMask = false;
			$this->Mask = '';
			$this->ValidRange = false;
			$this->Range = '';
			if (count($this->s_arr) < 4 ) {
				unset ($this->s_arr);
				return false;
			} else {
				foreach ($this->s_arr  as $key => $sectNumber){
					if (!strstr($sectNumber, '/') && $sectNumber >= 0 && $sectNumber < 256) {
						$IP_flags = $IP_flags + 1;
						$this->s_arr[$key] = dechex($sectNumber);
						if (strlen($this->s_arr[$key])==1) $this->s_arr[$key] = '0' .$this->s_arr[$key];
					}
				}
				if ($IP_flags != 4) return false;
				// is a mask, in binary form: 0's are allowed only on the right side (not in left or inside 1's!)
				$Bin_mask = decbin(hexdec($this->s_arr[0] .$this->s_arr[1] .$this->s_arr[2] .$this->s_arr[3]));
				$Bin_mask = str_repeat('0', 32 - strlen($Bin_mask)) .$Bin_mask;
				if (strstr($Bin_mask, "01") != "") return false;
				$this->Range = strspn($Bin_mask, '1');
				$this->ValidRange = true;
				$this->Mask = trim($IP);
				$this->ValidMask = true;
			}
		} else {
			$this->ValidIP = false;
			$this->IP = '';
			if (count($this->s_arr) != 4){
				unset ($this->s_arr);
				return false;
			} else {
				$temp = array();
				$temp = explode ('/', $this->s_arr[3]);
				$this->s_arr[3] = $temp[0];
				if (isset($temp[1])){
					// IP contain /nn as range mask
					$this->ValidMask = $this->BitsToIPMask($temp[1]);
					if ($this->ValidMask) {
						$this->Range = $temp[1];
						$this->ValidRange = true;
					} else {
						$this->Range='';
						$this->ValidRange = false;
					}
				} else {
					$this->Mask = '';
					$this->ValidMask = false;
					$this->Range='';
					$this->ValidRange = false;
				}
				foreach ($this->s_arr as $sectNumber){
					if (!strstr($sectNumber, '/') && $sectNumber >= 0 && $sectNumber < 256) $IP_flags = $IP_flags + 1;
				}
				if ($IP_flags != 4) return false;
				$this->ValidIP = true;
				$this->IP = $this->s_arr[0] .'.' .$this->s_arr[1] .'.' .$this->s_arr[2] .'.' .$this->s_arr[3];
			}
		}
		return true;
	}
	// build IP mask from a number of bits (max 32)
	function  BitsToIPMask($bits) {
		if ($bits < 0 || $bits > 32) return false;
		$maskbits = $bits;
		$i=0;
		$this->Mask = '';
		while ($maskbits >=8) {
			$this->Mask .= '255.';
			$maskbits = $maskbits - 8;
			$i++;
		}
		if ($maskbits > 0) {
			$binmask = str_repeat('1', $maskbits) . str_repeat('0', 8 - $maskbits);
			$this->Mask .= bindec($binmask) .'.';
			$i++;
		}
		while ($i < 4) {
			$this->Mask .= '0.';
			$i++;
		}
		$this->Mask = substr($this->Mask, 0, strlen($this->Mask)-1);
		return true;
	}
}
//=====================================================================================================================
//
// MISCELLANEOUS FUNCTIONS
//
//Get special DomainSettings (stored in Xmail server domain directory)
function getDomainSettings($domain){
	global $mail_server;
	// read the settings
	$filecontent = array();
	$data = $mail_server->cfgfileget('domains/' .$domain .'/phpxmailvars.tab');
	if (is_array($data)) {
		foreach ($data as $line){
			if (is_array($line) && $line != ''){
				$filecontent[str_replace('"', '', $line[0])] = str_replace('"', '', $line[1]);
			}
		}
	}
	//preset defaults
	if (!array_key_exists("MaxMailboxes", $filecontent)) {
		$filecontent['MaxMailboxes'] = 0;
	}
	if (!array_key_exists("MaxAliasPerUser", $filecontent)) {
		$filecontent['MaxAliasPerUser'] = 0;
	}
	ksort($filecontent);
	reset($filecontent);
	return $filecontent;
}
//Set special DomainSettings (stored in Xmail server domain directory)
function setDomainSettings($domain, $filecontent){
	global $mail_server;
	return $mail_server->cfgfileset('domains/' .$domain .'/phpxmailvars.tab', $filecontent );
}

/////////////////////
// SPAM IP ADDRESS FUNCTIONS
// List Spam IP (tested on Xmail 1.21)
function SpamIpListGet(){
	global $mail_server;
	$SpamIPList = array();
	$IP = new IP_object();
	$temp = array();
	//get the spammer file
	$vars = $mail_server->cfgfileget('spammers.tab');
	foreach ($vars as $spammer){
		if (is_array($spammer)) {
			$IP->trueIP($spammer[0], false);
			if ($IP->ValidIP) {
				$ActionCode = '1';
				if ($IP->ValidMask){
					// is in the form aaa.bbb.ccc.ddd/mm
					if (isset($spammer[1])) $ActionCode = $spammer[1];
				} else {
					$IP->trueIP($spammer[1], true);
					if (isset($spammer[2])) $ActionCode = $spammer[2];
				}
				if (!$IP->ValidMask) $IP->BitsToIPMask(32);
				$temp[0] = $IP->IP;
				$temp[1] = $IP->Mask;
				$ActionCode = str_replace('code=', '', $ActionCode);
				if ($ActionCode > 0) {$temp[2] = 0; $temp[3] = '';}
				if ($ActionCode == 0) {$temp[2] = 1; $temp[3] = '';}
				if ($ActionCode < 0) {$temp[2] = 2; $temp[3] = -$ActionCode;}
				// add to the output array: ignore multiple entries
				if (!key_exists($temp[0],$SpamIPList)) $SpamIPList[$temp[0]] = $temp;
			}
		}
	}
	ksort($SpamIPList);
	reset($SpamIPList);
	return $SpamIPList;
}

// Add a Spam IP
function SpamIpListAdd($SpamIp){
	// $SpamIp[0] must contain a valid IP address
	// $SpamIp[1] must contain a valid Mask range
	// $SpamIp[2] must contain a valid action (0-> blocked; 1->Auth Smtp; 2->Delayed)
	// $SpamIp[3] may contain a delay in seconds
	if (!is_array($SpamIp)) return false;
	$IP = new IP_object();
	// @var $IP IP_object
	$IP->trueIP($SpamIp[0], false);
	if ($IP->ValidIP) {
		$IP->trueIP($SpamIp[1], true);
		if ($IP->ValidMask) {
			$SpamIpList = SpamIpListGet();
			//if (!key_exists($IP->IP, $SpamIpList)){
			$SpamIpList[$IP->IP] = array($IP->IP, $IP->Mask, $SpamIp[2], $SpamIp[3]);
			return SpamIpListSet($SpamIpList);
			//}
		} else {
			return false;
		}
	} else {
		return false;
	}
}

// Remove a Spam IP
function SpamIpListDel($address){
	$IP = new IP_object();
	// @var $IP IP_object
	$IP->trueIP($address, false);
	if ($IP->ValidIP) {
		$SpamIpList = SpamIpListGet();
		if (key_exists($IP->IP, $SpamIpList)){
			unset($SpamIpList[$IP->IP]);
			return SpamIpListSet($SpamIpList);
		}
	} else {
		return false;
	}
}

// Save the whole Spam Addresses list
function SpamIpListSet($list) {
	global $mail_server;
	$SpamIpList = array();
	foreach ($list as $spamIp) {
		if (!empty($spamIp[3])){
			$ActionCode = 'code=' .-$spamIp[3];
		} else {
			$ActionCode = 'code=' .(1 - $spamIp[2]);
		}
		$SpamIpList[] = array('"' .$spamIp[0] .'"','"' .$spamIp[1] .'"','"' .$ActionCode .'"', );
	}
	if (is_array($SpamIpList) && count($SpamIpList) != 0 ){
		return ( $mail_server->cfgfileset('spammers.tab', $SpamIpList));
	}else{
		return ( $mail_server->cfgfileclear('spammers.tab', $SpamIpList)); // if no-data, do not delete file!
	}
}

/////////////////////
// SPAM ADDRESS FUNCTIONS
// List Spam Addresses (tested on Xmail 1.21)
function SpamListGet(){
	global $mail_server;
	$arr_User = array();		//usefull for sorting
	$arr_Domain = array();		//usefull for sorting
	$SpamAddressList = array();
	$i = 0;
	//get the spam-address file
	$vars = $mail_server->cfgfileget('spam-address.tab');
	foreach ($vars as $address){
		if ($address != '') {
			$SpamAddressList[$i] = explode('@', $address);
			if (!isset($SpamAddressList[$i][1])) {
				$SpamAddressList[$i][1] = $SpamAddressList[$i][0];
				$SpamAddressList[$i][0] = "";
			}
			array_push($arr_User, $SpamAddressList[$i][0]);
			array_push($arr_Domain, $SpamAddressList[$i][1]);
		}
		$i++;
	}
	// now sort for Domain, then for User
	array_multisort($arr_Domain, SORT_ASC, $arr_User, SORT_ASC, $SpamAddressList);
	return $SpamAddressList;
}

// Add a Spam Addresses
function SpamListAdd($address){
	if (empty($address)) return false;
	if (strpos($address, '@') === false) return false;
	$SpamAddress = explode('@', $address);
	if (!isset($SpamAddress[1])) {
		$SpamAddress[1] = $SpamAddress[0];
		$SpamAddress[0] = "";
	}
	if (strpos($SpamAddress[1], '.') === false) return false;
	//get the spam-address file
	$SpamAddressList = SpamListGet();
	if (in_array($SpamAddress, $SpamAddressList)) return false;
	//add the address to the array
	$SpamAddressList[] = $SpamAddress;
	return SpamListSet($SpamAddressList)	;
}

// Remove a Spam Addresses
function SpamListDel($address){
	if (empty($address)) return false;
	if (strpos($address, '@') === false) return false;
	$SpamAddress = explode('@', $address);
	if (!isset($SpamAddress[1])) {
		$SpamAddress[1] = $SpamAddress[0];
		$SpamAddress[0] = "";
	}
	//get the spam-address file
	$SpamAddressList = SpamListGet();
	$Key = array_search($SpamAddress, $SpamAddressList);
	if ($Key === false) return false;
	//unset the address to the array
	unset($SpamAddressList[$Key]);
	return SpamListSet($SpamAddressList)	;
}

// Save the whole Spam Addresses list
function SpamListSet($list) {
	global $mail_server;
	$SpamAddressList = array();
	foreach ($list as $spamaddr) {
		$SpamAddressList[] = array('"' .implode('@', $spamaddr) .'"');
	}
	if (is_array($SpamAddressList) && count($SpamAddressList) != 0 ){
		return ( $mail_server->cfgfileset('spam-address.tab', $SpamAddressList));
	}else{
		return ( $mail_server->cfgfileclear('spam-address.tab', $SpamAddressList)); // if no-data, do not delete file!
	}
}

?>