$fn=130;

D_sock=8;   //Sockeldurchmesser
D_knub=4.1; //Knubbeldurchmesser
H_sock=6;   //Sockelhöhe
H_knub=10.5   ;  //Knubbelhöhe 10 = Knuppelsockel=1.3mm
S_knub=0.5; //Schaftdurchmesser=Knubbeldurchmesser-0.5

//Sockel
H=H_sock-6;
color("SlateGray")
//color("WhiteSmoke")
translate([0,0,H*-1])
cylinder (h=H_sock, d=D_sock);

difference(){

//Knubbl
//color("SlateGray")
color("WhiteSmoke")    

union(){

translate([0,0,H_knub-0.7])
cylinder (h=1, d1=D_knub, d2=D_knub-2);

translate([0,0,H_knub-2.7])
cylinder (h=1, d1=D_knub-S_knub, d2=D_knub);

translate([0,0,H_knub-1.7])
cylinder (h=1, d1=D_knub, d2=D_knub);

//Schaft
translate([0,0,5.3])
cylinder (h=2.5, d=D_knub-S_knub);   
}


//Einschnitt
translate([-0.4,-2.5,6.3])
 cube([0.9,5,7]);
}

//Knuppel-Shaft messen V3 Höhe = 1.3mm
//translate([2,-3,6])
// cube([0.9,5,1.3]);