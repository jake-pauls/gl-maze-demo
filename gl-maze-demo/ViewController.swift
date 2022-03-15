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
            scene.swipe(Int32(SwipeUp))
        }
        else if changedDistance.y > 10 {
            scene.look(changedDistance)
            scene.swipe(Int32(SwipeDown))
        }
        else if changedDistance.x < -10 {
            scene.look(changedDistance)
            scene.swipe(Int32(SwipeLeft))
        }
        else if changedDistance.x > 10 {
            scene.look(changedDistance)
            scene.swipe(Int32(SwipeRight))
        }
    }
        
    @IBAction func toggle(_sender: Any) {
        scene.useFog = !scene.useFog
    }
}
