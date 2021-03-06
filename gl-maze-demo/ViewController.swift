//
//  ViewController.swift
//  gl-maze-demo
//
//  Created by Jake Pauls on 2022-03-08.
//

import GLKit

extension ViewController: GLKViewControllerDelegate {
    // Updates the scene each frame
    func glkViewControllerUpdate(_ controller: GLKViewController) {
        scene.update()
    }
}

class ViewController: GLKViewController {
    private var context: EAGLContext?
    private var scene: Scene!
    
    private func setupGLView() {
        context = EAGLContext(api: .openGLES3)
        EAGLContext.setCurrent(context)
        
        if let view = self.view as? GLKView, let context = context {
            view.context = context
            delegate = (self as GLKViewControllerDelegate)
            
            scene = Scene()
            scene.setup(view)
            scene.loadModels()
        }
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        setupGLView()
        
        let doubleTap = UITapGestureRecognizer(target:self,action: #selector(self.doDoubleTap(_:)))
        doubleTap.numberOfTapsRequired = 2;
        view.addGestureRecognizer(doubleTap)
        
        let pan = UIPanGestureRecognizer(target:self,action: #selector(self.doPan(_:)))
        view.addGestureRecognizer(pan)
    }
    
    // Renders the scene each frame
    override func glkView(_ view: GLKView, drawIn rect: CGRect) {
        scene.draw()
    }
    
    @objc func doDoubleTap(_ sender: UIGestureRecognizer) {
        scene.doDoubleTap()
    }
    
    @objc func doPan(_ sender: UIPanGestureRecognizer) {
        let changedDistance = sender.translation(in: view)
        
        if changedDistance.y < -10 {
            scene.look(changedDistance)
        }
        else if changedDistance.y > 10 {
            scene.look(changedDistance)
        }
        else if changedDistance.x < -10 {
            scene.look(changedDistance)
        }
        else if changedDistance.x > 10 {
            scene.look(changedDistance)
        }
    }
        
    @IBAction func toggleFog(_sender: Any) {
        scene.useFog = !scene.useFog
    }
    
    @IBOutlet weak var dayNightButton: UIButton!
    @IBAction func toggleDay(_sender: Any) {
        scene.isDay = !scene.isDay
        
        if scene.isDay {
            dayNightButton.setTitle("Night", for: UIControl.State.normal)
        } else {
            dayNightButton.setTitle("Day", for: UIControl.State.normal)
        }
    }
    
    @IBAction func toggleFlashlight(_sender: Any) {
        scene.useLight = !scene.useLight
    }
}
