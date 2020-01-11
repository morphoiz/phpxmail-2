<?PHP

// series of temporary settings must be used with PHPXmail
// the original value will be automatically restored at
// the end of the script execution

ini_set("session.use_cookie", "on");

ini_set("register_globals", "off");
ini_set("register_argc_argv", "off");
ini_set("register_long_arrays", "off");

ini_set("magic_quotes_gpc", "off");
ini_set("magic_quotes_runtime", "off");
ini_set("magic_quotes_sybase", "off");

ini_set("short_open_tags", "off");

?>