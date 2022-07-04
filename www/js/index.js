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

	htmlElement.style.cursor = `url('images/${name}-cursor.png'), auto`;
	document.body.style.cursor = `url('images/${name}-cursor.png'), auto`;
	document.body.style.backgroundImage = `url('images/${name}-background.png')`;

	document.body.style.backgroundSize = "cover";
	document.body.style.backgroundPosition = "center";
	document.body.style.backgroundRepeat = "no-repeat";
}