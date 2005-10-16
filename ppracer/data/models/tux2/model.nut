//
//Globals
//
local xlation_node = ":t2";
local rotation_node = ":t2";
local root = ":t2";

tux.root_node(2,root);

// Material Properties

tux.material("white_penguin2",
            [0.58,0.58,0.58],
			[0.2,0.2,0.2],
			50.0);
tux.material("black_penguin2",
			[0.1,0.1,0.9],
			[0.5,0.5,0.5],
			20.0);
tux.material("beak_color2",
			[0.64,0.54,0.06],
			[0.4,0.4,0.4],
			5);
tux.material("nostril_color2",
			[0.48039,0.318627,0.033725],
			[0.0,0.0,0.0],
			1);
tux.material("iris_color2",
			[0.01,0.01,0.01],
			[0.4,0.4,0.4],
			90.0);
		
// Define the geometry

// Helper function to make torso parts

function makeBody(l_parent)
{
    local torso_bl = l_parent + ":b";
    tux.sphere(l_parent, "b", 1.0);
    tux.surfaceproperty(torso_bl,"black_penguin2");
    tux.scale(torso_bl,[0,0,0],[0.95,1.0,0.8]);

    local torso_wh1 = l_parent + ":w1";
    tux.sphere(l_parent, "w1", 1.0);
    tux.surfaceproperty(torso_wh1,"white_penguin2");
    tux.translate(torso_wh1,[0.0,0.0,0.17]);
    tux.scale(torso_wh1,[0,0,0],[0.8,0.9,0.7]);
    tux.shadow(torso_wh1,"off");
}

tux.transform(":", "t2");
root = ":t2";
xlation_node = ":t2";

tux.transform(":t2", "r1");
rotation_node = ":t2:r1";
tux.scale(rotation_node,[0,0,0],[0.35,0.35,0.35]);

		

// Create torso

tux.transform(rotation_node, "tors_scl");
local torso_scl = rotation_node + ":tors_scl";
tux.scale(torso_scl,[0,0,0],[0.90,0.9,0.9]);

makeBody(torso_scl);

// Create shoulders

tux.transform(rotation_node,"t2");
local shoulder_xlate = rotation_node + ":t2";
tux.rotate(shoulder_xlate, "x", 10);
tux.translate(shoulder_xlate,[0,0.4,0.05]);

tux.transform(shoulder_xlate,"scl");
local shoulder_scale = shoulder_xlate + ":scl";
tux.scale(shoulder_scale,[0,0,0],[0.72,0.72,0.72]);

makeBody(shoulder_scale);

// Create neck

tux.transform(rotation_node,"t3");
local neck_xlate = rotation_node + ":t3";
tux.translate(neck_xlate,[0,0.9,0.07]);
tux.rotate(neck_xlate, "y", 90);

tux.transform(neck_xlate,"neckjt");
local neck_joint = neck_xlate + ":neckjt";
tux.neck(2,neck_joint);

tux.transform(neck_joint,"t4");
local neck_org = neck_joint + ":t4";
tux.rotate(neck_org, "y", -90);
// leave rotation point at origin

tux.transform(neck_org, "scl");
local neck_scale = neck_org + ":scl";
tux.scale(neck_scale,[0,0,0],[0.45,0.5,0.45]);

tux.sphere(neck_scale,"neck", 0.8);
local neck = neck_scale + ":neck";
tux.surfaceproperty(neck,"black_penguin2");

tux.sphere(neck_scale,"neckw",0.66);
local neckw = neck_scale + ":neckw";
tux.surfaceproperty(neckw,"white_penguin2");
tux.translate(neckw,[0,-0.08,0.35]);
tux.scale(neckw,[0,0,0], [0.8,0.9,0.7]);
tux.shadow(neckw, "off");

// create head

tux.transform(neck_org, "t5");
local head_xlate = neck_org + ":t5";
tux.translate(head_xlate, [0,0.3,0.07]);
tux.rotate(head_xlate, "y", 90);

tux.transform(head_xlate, "hdjtrot");
local head_joint_rot = head_xlate+ ":hdjtrot";	// head joint for rotation

tux.transform(head_joint_rot, "hdjt");
local head_joint = head_joint_rot + ":hdjt";	// head joint
tux.head(2,head_joint);

tux.transform(head_joint, "hdorg");
local head_org = head_joint + ":hdorg";
tux.rotate(head_org, "y", -90);
tux.translate(head_org,[0,0.20,0]);

tux.sphere(head_org, "hd", 1.0);
local head = head_org + ":hd";
tux.surfaceproperty(head, "black_penguin2");
tux.scale(head,[0,0,0], [0.42,0.5,0.42]);

// create beak

tux.sphere(head_org,"bk1", 0.8);
local beak1 = head_org + ":bk1";
tux.surfaceproperty(beak1, "beak_color2");
tux.translate(beak1, [0, -0.205, 0.3]);
tux.rotate(beak1, "x", 10);
tux.scale(beak1, [0,0,0], [0.23, 0.12, 0.4]);

tux.sphere(head_org, "bk3", 0.66);
local beak3 = head_org + ":bk3";
tux.surfaceproperty(beak3, "beak_color2");
tux.translate(beak3, [0, -0.23, 0.3]);
tux.rotate(beak3, "x", 10);
tux.scale(beak3, [0,0,0], [0.21, 0.17, 0.38]);
tux.shadow(beak3, "off");

// create eyes

tux.sphere(head_org, "le", 0.66);
local left_eye = head_org+ ":le";
tux.surfaceproperty(left_eye,"white_penguin2");
tux.translate(left_eye, [0.13, -0.03, 0.38]);
tux.rotate(left_eye,"y" ,18);
tux.rotate(left_eye,"z", 5);
tux.rotate(left_eye,"x", 5);
tux.scale(left_eye, [0, 0, 0], [0.1, 0.13, 0.03]);
tux.shadow(left_eye,"off");

tux.sphere(head_org, "re", 0.66);
local right_eye = head_org + ":re";
tux.surfaceproperty(right_eye, "white_penguin2");
tux.translate(right_eye, [-0.13, -0.03, 0.38]);
tux.rotate(right_eye, "y", -18);
tux.rotate(right_eye, "z", -5);
tux.rotate(right_eye, "x", 5);
tux.scale(right_eye, [0, 0, 0], [0.1, 0.13, 0.03]);
tux.shadow(right_eye, "off");


tux.sphere(head_org, "li", 0.66);
local left_iris = head_org + ":li";
tux.surfaceproperty(left_iris,"iris_color2");
tux.translate(left_iris,[0.12, -0.045, 0.40]);
tux.rotate(left_iris, "y", 18);
tux.rotate(left_iris, "z", 5);
tux.rotate(left_iris, "x", 5);
tux.scale(left_iris, [0, 0, 0], [0.055, 0.07, 0.03]);
tux.shadow(left_iris, "off");
tux.eye(left_iris,"left");

tux.sphere(head_org, "ri", 0.66);
local right_iris = head_org + ":ri";
tux.surfaceproperty(right_iris, "iris_color2");
tux.translate(right_iris, [-0.12, -0.045, 0.40]);
tux.rotate(right_iris, "y", -18);
tux.rotate(right_iris, "z", -5);
tux.rotate(right_iris, "x", 5);
tux.scale(right_iris, [0,0,0], [0.055, 0.07, 0.03]);
tux.shadow(right_iris, "off");
tux.eye(right_iris, "right");

// create Upper Arm

// left
tux.transform(shoulder_xlate,"luarmxlate");
local luparm_xlate = shoulder_xlate + ":luarmxlate";
tux.rotate(luparm_xlate, "y", 180);
tux.translate(luparm_xlate, [-0.56, 0.3, 0]);
tux.rotate(luparm_xlate, "z", 45);
tux.rotate(luparm_xlate, "x", 90);

tux.transform(luparm_xlate, "lshjt");
local lshoulder_joint = luparm_xlate + ":lshjt";       // left shoulder joint
tux.left_shoulder(2,lshoulder_joint);

tux.transform(lshoulder_joint, "luarmorg");
local luparm_org = lshoulder_joint + ":luarmorg";
tux.rotate(luparm_org, "x", -90);
tux.translate(luparm_org, [-0.22, 0, 0]);

tux.sphere(luparm_org, "uparm", 0.66);
local luparm= luparm_org +":uparm";
tux.surfaceproperty(luparm, "black_penguin2");
tux.scale(luparm, [0, 0, 0], [0.34, 0.1, 0.2]);

//right
tux.transform(shoulder_xlate, "ruarmxlate");
local ruparm_xlate= shoulder_xlate +":ruarmxlate";
tux.translate(ruparm_xlate, [-0.56, 0.3, 0]);
tux.rotate(ruparm_xlate, "z", 45);
tux.rotate(ruparm_xlate, "x", -90);

tux.transform(ruparm_xlate, "rshjt");                // right shoulder joint
local rshoulder_joint= ruparm_xlate +":rshjt";
tux.right_shoulder(2,rshoulder_joint);

tux.transform(rshoulder_joint, "ruarmorg");
local ruparm_org= rshoulder_joint +":ruarmorg";
tux.rotate(ruparm_org, "x", 90);
tux.translate(ruparm_org, [-0.22, 0, 0]);

tux.sphere(ruparm_org, "uparm", 0.66);
local ruparm = ruparm_org +":uparm";
tux.surfaceproperty(ruparm,"black_penguin2");
tux.scale(ruparm, [0, 0, 0], [0.34, 0.1, 0.2]);

//
// Create forearm
//

// left
tux.transform(luparm_org, "llarmxlate");
local llowarm_xlate= luparm_org +":llarmxlate";
tux.translate(llowarm_xlate, [-0.23, 0, 0]);
tux.rotate(llowarm_xlate, "z", 20);
tux.rotate(llowarm_xlate, "x", 90);

tux.transform(llowarm_xlate, "lelbjt");
local lelbow_joint= llowarm_xlate +":lelbjt"        // left elbow joint

tux.transform(lelbow_joint, "llarmorg");
local llowarm_org= lelbow_joint +":llarmorg";
tux.rotate(llowarm_org, "x", -90);
tux.translate(llowarm_org, [-0.19, 0, 0]);

tux.sphere(llowarm_org, "llowarm", 0.66);
local llowarm= llowarm_org + ":llowarm";
tux.surfaceproperty(llowarm, "black_penguin2");
tux.scale(llowarm, [0, 0, 0], [0.30, 0.07, 0.15]);

// right
tux.transform(ruparm_org, "rlarmxlate");
local rlowarm_xlate= ruparm_org +":rlarmxlate";
tux.translate(rlowarm_xlate, [-0.23, 0, 0]);
tux.rotate(rlowarm_xlate, "z", 20);
tux.rotate(rlowarm_xlate, "x", -90);

tux.transform(rlowarm_xlate, "relbjt");              // right elbow joint
local relbow_joint= rlowarm_xlate +":relbjt";

tux.transform(relbow_joint, "rlarmorg");
local rlowarm_org= relbow_joint +":rlarmorg";
tux.rotate(rlowarm_org, "x", 90);
tux.translate(rlowarm_org, [-0.19, 0, 0]);

tux.sphere(rlowarm_org, "rlowarm", 0.66);
local rlowarm= rlowarm_org +":rlowarm";
tux.surfaceproperty(rlowarm, "black_penguin2");
tux.scale(rlowarm, [0, 0, 0], [0.30, 0.07, 0.15]);

//
// Create hands
//

// left
tux.transform(llowarm_org, "lhandxlate");
local lhand_xlate= llowarm_org +":lhandxlate";
tux.translate(lhand_xlate, [-0.24, 0, 0]);
tux.rotate(lhand_xlate, "z", 20);
tux.rotate(lhand_xlate, "x", 90);

tux.transform(lhand_xlate, "lwrtjt");
local lwrist_joint= lhand_xlate +":lwrtjt"          // left wrist joint

tux.transform(lwrist_joint, "lhndorg");
local lhand_org= lwrist_joint +":lhndorg";
tux.rotate(lhand_org, "x", -90);
tux.translate(lhand_org, [-0.10, 0, 0]);

tux.sphere(lhand_org, "lhand", 0.5);
local lhand= lhand_org +":lhand";
tux.surfaceproperty(lhand, "black_penguin2");
tux.scale(lhand, [0, 0, 0], [0.12, 0.05, 0.12]);

// right
tux.transform(rlowarm_org, "rhandxlate");
local rhand_xlate= rlowarm_org +":rhandxlate";
tux.translate(rhand_xlate, [-0.24, 0, 0]);
tux.rotate(rhand_xlate, "z", 20);
tux.rotate(rhand_xlate, "x", -90);

tux.transform(rhand_xlate, "rwrtjt");
local rwrist_joint= rhand_xlate +":rwrtjt"          // right wrist joint

tux.transform(rwrist_joint, "rhndorg");
local rhand_org= rwrist_joint +":rhndorg";
tux.rotate(rhand_org, "x", 90);
tux.translate(rhand_org, [-0.10, 0, 0]);

tux.sphere(rhand_org, "rhand", 0.5);
local rhand= rhand_org +":rhand";
tux.surfaceproperty(rhand, "black_penguin2");
tux.scale(rhand, [0, 0, 0], [0.12, 0.05, 0.12]);

//
// Make thighs
// 

//left
tux.transform(rotation_node, "lthgxlate");
local lthigh_xlate= rotation_node +":lthgxlate";
tux.rotate(lthigh_xlate, "y", 180);
tux.translate(lthigh_xlate, [-0.28, -0.8, 0]);
tux.rotate(lthigh_xlate, "y", 110);

tux.transform(lthigh_xlate, "lhipjt");               // left hip joint
local lhip_joint= lthigh_xlate +":lhipjt";
tux.left_hip(2,lhip_joint);

tux.transform(lhip_joint, "lthgorg");
local lthigh_org= lhip_joint +":lthgorg";
tux.rotate(lthigh_org, "y", -110);
tux.translate(lthigh_org, [ 0, -0.1, 0 ]);

tux.sphere(lthigh_org ,"lthigh", 0.5);
local lthigh= lthigh_org +":lthigh";
tux.surfaceproperty(lthigh, "beak_color2");
tux.scale(lthigh, [0, 0, 0], [0.07, 0.30, 0.07]);

tux.sphere(lthigh_org, "lhipball", 0.5);
local lhipball= lthigh_org +":lhipball";
tux.surfaceproperty(lhipball, "black_penguin2");
tux.translate(lhipball, [0.0, 0.05, 0.0]);
tux.scale(lhipball, [0, 0, 0], [0.09, 0.18, 0.09]);
tux.shadow(lhipball,"off");

//right
tux.transform(rotation_node, "rthgxlate");
local rthigh_xlate= rotation_node +":rthgxlate";
tux.translate(rthigh_xlate, [-0.28, -0.8, 0]);
tux.rotate(rthigh_xlate, "y", -110);

tux.transform(rthigh_xlate, "rhipjt");               // right hip joint
local rhip_joint= rthigh_xlate +":rhipjt";
tux.right_hip(2,rhip_joint);

tux.transform(rhip_joint, "rthgorg");
local rthigh_org= rhip_joint +":rthgorg";
tux.rotate(rthigh_org, "y", 110);
tux.translate(rthigh_org, [ 0, -0.1, 0 ]); 

tux.sphere(rthigh_org, "rthigh", 0.5);
local rthigh= rthigh_org +":rthigh";
tux.surfaceproperty(rthigh, "beak_color2");
tux.scale(rthigh, [0, 0, 0], [0.07, 0.30, 0.07]);

tux.sphere(rthigh_org, "rhipball", 0.5);
local rhipball= rthigh_org +":rhipball";
tux.surfaceproperty(rhipball, "black_penguin2");
tux.translate(rhipball, [0.0, 0.05, 0.0]);
tux.scale(rhipball, [0, 0, 0], [0.09, 0.18, 0.09]);
tux.shadow(rhipball,"off");

//
// Make calves
//

//left
tux.transform(lthigh_org, "lclfxlate");
local lcalf_xlate= lthigh_org +":lclfxlate";
tux.translate(lcalf_xlate, [0, -0.21, 0]);
tux.rotate(lcalf_xlate, "y", 90);

tux.transform(lcalf_xlate, "lkneejt");               // left knee joint
local lknee_joint= lcalf_xlate +":lkneejt";    
tux.left_knee(2,lknee_joint);

tux.transform(lknee_joint, "lclforg");
local lcalf_org= lknee_joint +":lclforg";
tux.rotate(lcalf_org, "y", -90);
tux.translate(lcalf_org, [ 0, -0.13, 0 ]); 

tux.sphere(lcalf_org, "lcalf", 0.5);
local lcalf= lcalf_org +":lcalf";
tux.surfaceproperty(lcalf, "beak_color2");
tux.scale(lcalf, [0, 0, 0], [0.06, 0.18, 0.06]);

//right
tux.transform(rthigh_org, "rclfxlate");
local rcalf_xlate= rthigh_org +":rclfxlate";
tux.translate(rcalf_xlate, [0, -0.21, 0]);
tux.rotate(rcalf_xlate, "y", -90);

tux.transform(rcalf_xlate, "rkneejt");               // right knee joint
local rknee_joint= rcalf_xlate +":rkneejt";
tux.right_knee(2,rknee_joint);

tux.transform(rknee_joint, "rclforg");
local rcalf_org= rknee_joint +":rclforg";
tux.rotate(rcalf_org, "y", 90);
tux.translate(rcalf_org, [ 0, -0.13, 0 ]); 

tux.sphere(rcalf_org, "rcalf", 0.5);
local rcalf= rcalf_org +":rcalf";
tux.surfaceproperty(rcalf, "beak_color2");
tux.scale(rcalf, [0, 0, 0], [0.06, 0.18, 0.06]);

//
// Create feet
//

// helper function to make feet
function makeFoot(l_parent)
{
	tux.transform(l_parent, "footorg");

    local foot_org= l_parent +":footorg";
    tux.translate(foot_org, [ -0.13, 0, 0 ]);
    tux.surfaceproperty(foot_org, "beak_color2");
    tux.scale(foot_org, [ 0, 0, 0 ], [ 1.1, 1.0, 1.3 ]);

    tux.sphere(foot_org, "base", 0.66);
    local foot_base= foot_org +":base";
    tux.scale(foot_base, [0, 0, 0], [0.25, 0.08, 0.18]);

    tux.sphere(foot_org, "toe1", 0.66);
    local toe1= foot_org +":toe1";
    tux.surfaceproperty(toe1,"beak_color2");
    tux.translate(toe1, [-0.07, 0, 0.1]);
    tux.rotate(toe1, "y", 30);
    tux.scale(toe1, [0, 0, 0], [0.27, 0.07, 0.11]);

    tux.sphere(foot_org, "toe2", 0.66);
    local toe2= foot_org +":toe2";
    tux.surfaceproperty(toe2, "beak_color2");
    tux.translate(toe2, [-0.07, 0, -0.1]);
    tux.rotate(toe2, "y", -30);
    tux.scale(toe2, [0, 0, 0], [0.27, 0.07, 0.11]);

    tux.sphere(foot_org, "toe3", 0.66);
    local toe3= foot_org +":toe3";
    tux.surfaceproperty(toe3, "beak_color2");
    tux.translate(toe3, [-0.08, 0, 0]);
    tux.scale(toe3, [0, 0, 0], [0.27, 0.07, 0.10]);
}

// left foot
tux.transform(lcalf_org, "lftxlate");
local lfoot_xlate= lcalf_org +":lftxlate";
tux.translate(lfoot_xlate, [ 0, -0.18, 0 ]);
tux.rotate(lfoot_xlate, "y", -50);

tux.transform(lfoot_xlate, "lankjt");
local lankle_joint= lfoot_xlate +":lankjt";          // left ankle joint
tux.left_ankle(2,lankle_joint);

makeFoot(lankle_joint);

// right foot
tux.transform(rcalf_org, "rftxlate");
local rfoot_xlate= rcalf_org +":rftxlate";
tux.translate(rfoot_xlate, [ 0, -0.18, 0 ]);
tux.rotate(rfoot_xlate, "y", 50);

tux.transform(rfoot_xlate, "rankjt");
local rankle_joint= rfoot_xlate +":rankjt";          // right ankle joint
tux.right_ankle(2,rankle_joint);

makeFoot(rankle_joint);

//
// Create Tail
//
tux.transform(rotation_node, "tailxlate");
local tail_xlate= rotation_node +":tailxlate";
tux.translate(tail_xlate, [0, -0.4, -0.5]);
tux.rotate(tail_xlate, "x", -60);

tux.transform(tail_xlate, "tail_joint");
local tail_joint= tail_xlate +":tail_joint";        // tail joint
tux.tail(2,tail_joint);

tux.transform(tail_joint, "tail_orig");
local tail_orig= tail_joint +":tail_orig";
tux.translate(tail_orig, [0, 0.15, 0]);

tux.sphere(tail_orig, "tail", 0.5);
local tail= tail_orig +":tail";
tux.surfaceproperty(tail, "black_penguin2");
tux.scale(tail, [0, 0, 0], [0.2, 0.3, 0.1]);
