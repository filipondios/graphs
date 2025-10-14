use std::collections::HashMap;
use std::fs::File;
use std::io::{BufReader, BufRead};

// Graph represented as an adjacency map <a -> [b, c, ...]>
// An edge is represented as graph[a] = <b, weight> 
type Graph = HashMap<String, HashMap<String, usize>>;


pub fn from_list(edge_list: &[(&str, &str, usize)]) -> Graph {
    let mut graph = Graph::new();

    edge_list.iter().for_each(|&(a, b, weight)| {
        // Insert connection (a -> b) = weight
        graph.entry(a.to_string())
            .or_insert_with(HashMap::new)
            .insert(b.to_string(), weight);

        // Insert connection (b -> a) = weight
        graph.entry(b.to_string())
            .or_insert_with(HashMap::new)
            .insert(a.to_string(), weight);
    });
    graph
}

pub fn from_file(file_path: &str) -> Result<Graph, &'static str> {
    let file = File::open(file_path).map_err(|_| "unable to open file")?;
    let reader = BufReader::new(file);
    let mut graph = Graph::new();

    for line in reader.lines() {
        let line = line.map_err(|_| "error reading a line")?;
        let parts: Vec<&str> = line.trim().split(' ').collect();

        if parts.len() != 3 {
            return Err("invalid syntax");
        }

        // Valid line format: "a b weight"
        let a = parts[0].to_string();
        let b = parts[1].to_string();
        let weight = parts[2].parse::<usize>()
            .map_err(|_| "invalid edge weight")?;

        // Insert connection (a -> b) = weight
        graph.entry(a.clone())
            .or_insert_with(HashMap::new)
            .insert(b.clone(), weight);
        
        // Insert connection (b -> a) = weight
        graph.entry(b.clone())
            .or_insert_with(HashMap::new)
            .insert(a.clone(), weight);
    }
    Ok(graph)
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_from_list_basic() {
        let edges = [("a", "b", 3), ("a", "c", 5), ("b", "c", 1)];
        let g = from_list(&edges);
        
        assert_eq!(g["a"]["b"], 3);
        assert_eq!(g["b"]["a"], 3);
        
        assert_eq!(g["a"]["c"], 5);
        assert_eq!(g["c"]["a"], 5);

        assert_eq!(g["b"]["c"], 1);
        assert_eq!(g["c"]["b"], 1);       
    }

    #[test]
    fn test_from_file_correct() {
        // Basic example. All is right
        let basic = from_file("tests/edges_basic.txt");
        assert!(basic.is_ok());
        
        let g = basic.unwrap();
        assert_eq!(g["a"]["b"], 3);
        assert_eq!(g["a"]["b"], 3);
        
        assert_eq!(g["a"]["c"], 5);
        assert_eq!(g["c"]["a"], 5);

        assert_eq!(g["b"]["c"], 1);
        assert_eq!(g["c"]["b"], 1);

        // Duplicate example. Multiple definitions
        let duplicates = from_file("tests/edges_duplicate.txt");
        assert!(duplicates.is_ok());

        let g = duplicates.unwrap();
        assert_eq!(g["a"]["b"], 3);
        assert_eq!(g["b"]["a"], 3);
    }
    
    #[test]
    fn test_from_file_invalid() {
        // Example with a non usize weight 
        let invalid_weight = from_file("tests/edges_invalid_weight.txt");
        assert!(invalid_weight.is_err());

        // Example without an edge weight (a -> b) = ?
        let result = from_file("tests/edges_invalid_no_weight.txt");
        assert!(result.is_err());
    }
}
