//////////////////////////
// 	HOW TO RUN	//
//////////////////////////

1. EXTRACT the project folder 'SubdivisionSurfaces' 
2. COMPILE the program using the command line 'make'
3. RUN the program using the command line 'boilerplate.out'

//////////////////////////
// 	HOW TO USE	//
//////////////////////////

mouse:
Right_Click + drag 		Pan camera up/down/left/right centered on mesh
Left_Click			Selects/Unselects a face
Left_Click + drag		Multiple face selection (adjacest faces)
Left_Click + shift + (drag)	Multiple face selection (seperate faces)
Scroll Wheel			Zooms in to and away from the mesh

keyboard:
[space]		apply adaptive incremental catmull-clark subdivision on selected faces
[up/down]	stretch selected faces outwards/inwards
[left/right]	pull selected faces upwards/downwards
[o]		opens a .obj file (requires command line input)
[p]		saves current mesh to a .obj file (requires command line input)

//////////////////////////
// 	WARNINGS	//
//////////////////////////

- Boundary selection is not accounted for, operations on selected faces that are on boundary will segfault
- There is an edge case we could not find:
	- after subdividing several faces several times, if you select another region that has new 		faces that have been subdivided, there is a really small chance of seg fault. and we were 		unable to consistantly replicate this seg fault to find what the edge case is

