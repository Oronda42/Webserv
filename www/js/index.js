const loginButton = document.getElementById('login-button');
const loginForm = document.getElementById('login-form');

loginButton.addEventListener("click", () => {
	loginForm.style.display = "flex";
});

window.onclick = function(event) {
	// Check if event.target is not a child of loginform
	if (!loginForm.contains(event.target) && event.target != loginButton) {
		loginForm.style.display = "none";
	}
}

function cardClicked(name)
{
	// Change cursor in website (body) and outside on bottom (html)
	let htmlElement = document.querySelector("html");

	switch(name)
	{
		case "oronda":
			htmlElement.style.cursor = "url('images/oronda-cursor.png'), auto";
			document.body.style.cursor = "url('images/oronda-cursor.png'), auto";
			document.body.style.backgroundImage = "url('images/oronda-background.jpg')";
			break;

		case "mframbou":
			htmlElement.style.cursor = "url('images/mframbou-cursor.png'), auto";
			document.body.style.cursor = "url('images/mframbou-cursor.png'), auto";
			document.body.style.backgroundImage = "url('images/mframbou-background.jpg')";
			break;

		case "dsamain":
			htmlElement.style.cursor = "url('images/dsamain-cursor.png'), auto";
			document.body.style.cursor = "url('images/dsamain-cursor.png'), auto";
			document.body.style.backgroundImage = "url('images/dsamain-background.png')";
			break;
	}
}