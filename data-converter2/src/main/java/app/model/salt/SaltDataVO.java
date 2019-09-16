package app.model.salt;

import app.model.connection.ConnectionVO;
import app.model.edge.EdgeVO;
import app.model.node.NodeVO;
import app.model.tss.TssVO;

import java.util.HashMap;
import java.util.List;

public class SaltDataVO {
    private HashMap<String, EdgeVO> edgeMap;
    private HashMap<String, NodeVO> nodeMap;
    private HashMap<String, TssVO> tssMap;
    private List<ConnectionVO> connectionList;

    public HashMap<String, EdgeVO> getEdgeMap() {
        return edgeMap;
    }

    public void setEdgeMap(HashMap<String, EdgeVO> edgeMap) {
        this.edgeMap = edgeMap;
    }

    public HashMap<String, NodeVO> getNodeMap() {
        return nodeMap;
    }

    public void setNodeMap(HashMap<String, NodeVO> nodeMap) {
        this.nodeMap = nodeMap;
    }

    public HashMap<String, TssVO> getTssMap() {
        return tssMap;
    }

    public void setTssMap(HashMap<String, TssVO> tssMap) {
        this.tssMap = tssMap;
    }

    public List<ConnectionVO> getConnectionList() {
        return connectionList;
    }

    public void setConnectionList(List<ConnectionVO> connectionList) {
        this.connectionList = connectionList;
    }
}
