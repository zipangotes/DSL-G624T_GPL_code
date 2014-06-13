function encrypt(str)
{
	var result = "";
	var	escaped;

	escaped = escape(str);
	for(i = 0; i < escaped.length; i++)
		result += escaped.charCodeAt(i) - 23;
	return result;
}

function decrypt(str)
{
	var result = "";
	var num;
	for(i = 0; i < str.length; i += 2)
	{
		num = parseInt(str.substr(i,[2])) + 23;
		num = unescape('%' + num.toString(16));
		result += num;
	}
	return unescape(result);
}

