# RoseScript Programming Language

RoseScript is a programming language designed to be simple and C# Like

```
// example.rose
module Utils.Math

string start_cmd = "Type 'start' to run the loop: "

// Basic string-check simulation 
// (Note: Currently logic handles numeric evaluation)
val run_permission = 1 

if run_permission == 1 {
    printl("--- Starting Circle Calculation ---")
    
    val radius = 1
    val max_radius = 4

    while radius < max_radius {
        printl("Radius:")
        printl(radius)
        
        // Use 'factor' from the loaded Utils.Math module
        val diameter = radius * factor
        printl("Diameter is:")
        printl(diameter)
        
        // Increment radius
        val radius = radius + 1
    }
}

if run_permission == 0 {
    printl("Loop was skipped.")
}

printl("Execution finished.")
```

and yes this a simple language its just simple thats it
