function getCookie(cname)
{
	let name = cname + "=";
	let decodedCookie = decodeURIComponent(document.cookie);
	let ca = decodedCookie.split(';');

	for(let i = 0; i <ca.length; i++)
	{
		let c = ca[i];

		while (c.charAt(0) == ' ')
			c = c.substring(1);

		if (c.indexOf(name) == 0)
			return c.substring(name.length, c.length);
	}
	return null;
}

window.onload = function()
{
	console.log("test");
	if (getCookie("webserv_username") != null)
		document.getElementById("logged-in-username").innerHTML = "Hello " + getCookie("webserv_username");
}