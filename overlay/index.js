var player_positions = [];

function setup()
{
    createCanvas(1920,1080);
	textFont('Helvetica');
    background(0);
}

function draw()
{
    fill(255, 20, 20);
    stroke(255,20,20);
    background(0);
    
    for(var i = 0; i < player_positions.length; i++)
    {
        strokeWeight(3);
		stroke(255,20,20);
        var player = player_positions[i];
        //circle(player.m_vecFeet.x, player.m_vecFeet.y, 20);
        //circle(player.m_vecHead.x, player.m_vecHead.y, 20);
		var height = player.m_vecHead.y - player.m_vecFeet.y;
		var width = height * 0.2;

		line(player.m_vecHead.x - width, player.m_vecHead.y, player.m_vecHead.x + width, player.m_vecHead.y);
		line(player.m_vecFeet.x - width, player.m_vecFeet.y, player.m_vecFeet.x + width, player.m_vecFeet.y);
		
        line(player.m_vecHead.x - width, player.m_vecHead.y, player.m_vecFeet.x - width, player.m_vecFeet.y);
		line(player.m_vecHead.x + width, player.m_vecHead.y, player.m_vecFeet.x + width, player.m_vecFeet.y);
		
		stroke(20,255,20);
		if(player.m_iHealth < 75)
		{
			stroke(255,127,20);
		}
		if(player.m_iHealth < 25)
		{
			stroke(255,20,20);
		}
		
		line(player.m_vecFeet.x + width - 10, player.m_vecFeet.y, player.m_vecHead.x + width - 10, player.m_vecFeet.y + (player.m_iHealth/100 * height));
		
		//strokeWeight(0);
		//textSize(32);
		//text(`m_iHealth: ${player.m_iHealth}`, player.m_vecHead.x + width,  player.m_vecHead.y - 10);
    }
	
	strokeWeight(2);
	stroke(255);
	line(960, 530, 960, 550);
	line(950, 540, 970, 540);
}

let socket = new WebSocket("ws://127.0.0.1:9002");

socket.onmessage = function(event) {
    player_positions = JSON.parse(event.data);
	if(!player_positions)
	{
		player_positions = {};
	}
}