function getCookieValueByName(cookieName)
{
	decodedCookies = decodeURIComponent(document.cookie).split(';');
	cookieName += "=";
	for (let i = 0 ; i < decodedCookies.length ; i++)
	{
		startIndex = (decodedCookies[i][0] == ' ');
		if (decodedCookies[i].indexOf(cookieName) == startIndex)
			return (decodedCookies[i].substring(cookieName.length));
	}
	return (null);
}

window.onload = function()
{
	cookieValue = getCookieValueByName("webservUsername");
	if (cookieValue != null) {
		const p = document.querySelector('p.display-none');
		p.classList.toggle("display-none")
		p.innerHTML += " " + cookieValue + ' !';

		document.getElementById("cookie_form").classList.toggle("display-none");
	}
}
