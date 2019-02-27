var player1 = prompt("type your name, you will be Blue");
var player2 = prompt("type your name, you will be Red");
var coloum = 0;
var bttn = $('.b1');
var redcss = {
  'background' : 'red'
}


$('h3').text(player1+": it is your turn, please pick a column to drop your blue chip.");

$('h3').text(player2+": it is your turn, please pick a column to drop your blue chip.");

function lastCircle(){
    while (coloum <= 6){
      console.log("OK");
        $('td').click(function(){
          bttn.css(redcss)
        })
  }
} // func end
