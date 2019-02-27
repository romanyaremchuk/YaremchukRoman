var FirstName = prompt("Name");
var LastName  = prompt("Last Name");
var Age       = prompt("Age?");
var hight     = prompt("Hight cm?");
var petName   = prompt("Pet?");
alert("thanks");
if (FirstName[0] == LastName[0]){
  if (Age >= 20 && Age <= 30){
    if (hight > 170){
      if (petName[petName.length - 1] == "y"){
        console.log("HAHA");
      }
    }
  }
}
