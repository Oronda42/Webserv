const loginButton = document.getElementById('login-button');
const loginForm = document.getElementById('login-form');


loginButton.addEventListener("click", () => {
	loginForm.style.display = "flex";
});

function hasParent(element, parentSelector) {
    var potentialParents = document.querySelectorAll(parentSelector);
    for(i in potentialParents)
	{
		if(potentialParents[i].contains(element))
        	return potentialParents[i];
	}
	return false;
}

window.onclick = function(event) {
	// Check if event.target is not a child of loginform
	console.log(event.target);
	if (event.target != loginForm && event.target != loginButton && !event.target.contains("login")) {
		loginForm.style.display = "none";
	}
}